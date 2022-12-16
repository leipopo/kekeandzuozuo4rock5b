#include "fanctrl.hpp"

using namespace std;



void writefile(const char *path, string data)
{
    ofstream file;
    file.open(path, ios::out);
    file << data;
    file.close();
}

string readfile(const char *path)
{
    ifstream file;
    file.open(path, ios::in);
    string data;
    file >> data;
    file.close();
    return data;
}

void string2char(string str, char *ch)
{
    int i;
    for (i = 0; i < str.length(); i++)
    {
        ch[i] = str[i];
    }
    ch[i]='\0';
}

void FAN ::readconfigfile(const char *path)
{
    fstream file;
    file.open(path, ios::in | ios::out);
    int begin, end;
    string line;
    getline(file, line);
    begin = line.find_first_of('"');
    end = line.find_last_of('"');

    if (line.substr(begin + 1, end - begin - 1) == "1")
    {
        file.seekp(begin + 1, ios::beg);
        file.write("0", 1);
        file.close();
        file.open(path, ios::in);
        while (!file.eof())
        {
            getline(file, line);
            end = line.find("=");
            string item = line.substr(0, end);
            if (item == "obj")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                string obj_temp = line.substr(begin + 1, end - begin - 1);
                if (obj_temp == "soc")
                {
                    this->obj = soc;
                }
                // else if(obj_temp=="ssd")
                // {
                //     this->obj=ssd;
                // }
            }
            else if (item == "exp_temp")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                this->exp_temp = stof(line.substr(begin + 1, end - begin - 1));
            }
            else if (item == "wall_temp")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                this->wall_temp = stof(line.substr(begin + 1, end - begin - 1));
            }
            else if (item == "fan_maxpwm")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                this->fan_maxpwm = stoi(line.substr(begin + 1, end - begin - 1));
            }
            else if (item == "fan_minpwm")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                this->fan_minpwm = stoi(line.substr(begin + 1, end - begin - 1));
            }
            else if (item == "fan_maxpower")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                this->fan_maxpower = stof(line.substr(begin + 1, end - begin - 1));
            }
            else if (item == "fan_mod")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                this->fan_mod = stoi(line.substr(begin + 1, end - begin - 1));
            }
            else if ((item == "fan_pwm") && this->fan_mod == manualmod)
            {

                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                this->fan_pwm = stoi(line.substr(begin + 1, end - begin - 1));
            }
            else if (item == "pwm_period")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                this->pwm_period = stoi(line.substr(begin + 1, end - begin - 1));
            }
            else if (item == "pwm_chip")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                string2char(line.substr(begin + 1, end - begin - 1), this->pwm_chip);
                // cout << this->pwm_chip << endl;
            }
            else if (item == "pwm_channel")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                string2char(line.substr(begin + 1, end - begin - 1), this->pwm_channel);
                // cout << this->pwm_channel << endl;
            }
        }
    }
    file.close();
}

void init(const char *path,FAN *fan)
{
    fstream file;
    file.open(path, ios::in | ios::out);
    int begin, end;
    string line;
    getline(file, line);
    begin = line.find_first_of('"');
    end = line.find_last_of('"');
    file.seekp(begin + 1, ios::beg);
    file.write("1", 1);
    file.close();

    fan->readconfigfile(path);

    const char *chip = fan->pwm_chip;
    const char *channel = fan->pwm_channel;
    string const &finalpath1 = pwm_path + string(chip);
    string finalpath2 = finalpath1 + string(channel);
    string export_path =finalpath1+"/export";
    string duty_cycle_path = finalpath2 + "/duty_cycle";
    string period_path = finalpath2 + "/period";
    string enable_path = finalpath2 + "/enable";
    string polarity_path = finalpath2 + string("/polarity");

    //cout << export_path << endl;
    string2char(export_path, fan->pwm_export_path);
    //cout << this->pwm_export_path << endl;
    string2char(duty_cycle_path, fan->pwm_dutycycle_path);
    string2char(period_path, fan->pwm_period_path);
    string2char(enable_path, fan->pwm_enable_path);
    string2char(polarity_path, fan->pwm_polarity_path);

 
    //cout << this->pwm_polarity_path << endl;

    if (fan->obj == soc)
    {
        string temp = string(soc_temp_path);
        string2char(temp,fan->objtemp_path);

    }
    // else if(this->obj==ssd)
    // {
    //     this->objtemp_path=ssd_temp_path;
    // }
 
    writefile(fan->pwm_enable_path, "1");
    usleep(1000*1000);
    writefile(fan->pwm_dutycycle_path, to_string(fan->fan_pwm));
    writefile(fan->pwm_period_path, to_string(fan->pwm_period));
    writefile(fan->pwm_export_path, "0");
    writefile(fan->pwm_polarity_path, "normal");
}

int FAN::power2pwm(float power)
{
    int pwm = (this->fan_maxpwm - this->fan_minpwm) * fmin(power, this->fan_maxpower) + this->fan_minpwm;
    return pwm;
}

void FAN::boost()
{
    if (this->fan_pwm == 0)
    {
        writefile(pwm_dutycycle_path, to_string(this->pwm_period));
        usleep(500 * 1000); // 500ms
    }
}

void FAN::pwmcalc()
{
    string temp = readfile(this->objtemp_path);
    this->obj_temp = stof(temp) / 1000.f;
    switch (this->fan_mod)
    {
    case switchmod:
    {
        if (this->obj_temp > this->wall_temp)
        {
            this->boost();
            this->fan_pwm = this->power2pwm(this->fan_maxpower);
        }
        else if (this->obj_temp < this->exp_temp)
        {
            this->fan_pwm = 0;
        }
    }
    break;

    case linermod:
    {
        if (this->obj_temp > this->exp_temp + (this->wall_temp - this->exp_temp) * 3 / 5)
        {
            this->boost();
            this->fan_pwm = this->power2pwm((this->obj_temp - this->exp_temp) / (this->wall_temp - this->exp_temp));
        }
        else if (this->obj_temp < this->exp_temp)
        {
            this->fan_pwm = 0;
        }
    }
    break;

    default:
        break;
    }
}

void FAN::setpwmdev()
{
    string data;
    data = to_string(this->fan_pwm);
    writefile(this->pwm_dutycycle_path, data);
}

int main()
{
    FAN socfan, ssdfan;
    init(soc_fan_configfile_path,&socfan);
    init(ssd_fan_configfile_path,&ssdfan);
    while (1)
    {
        socfan.readconfigfile(soc_fan_configfile_path);
        ssdfan.readconfigfile(ssd_fan_configfile_path);
        socfan.pwmcalc();
        ssdfan.pwmcalc();
        socfan.setpwmdev();
        ssdfan.setpwmdev();
        usleep(1000 * 20);
    }
}
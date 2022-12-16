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
                this->pwm_chip = line.substr(begin + 1, end - begin - 1).c_str();
                cout<<this->pwm_chip<<endl;
            }
            else if (item == "pwm_channel")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                this->pwm_channel = line.substr(begin + 1, end - begin - 1).c_str();
                cout<<this->pwm_channel<<endl;
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
        }
    }
    file.close();
}

void FAN::init(const char *path)
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

    this->readconfigfile(path);
    stringstream temp;
    temp << pwm_path << this->pwm_chip << this->pwm_channel << "/duty_cycle";
    string finalpath = temp.str();
    //this->pwm_dutycycle_path = finalpath.append(&(this->pwm_chip)).append("/duty_cycle").c_str();
    cout<<finalpath<<endl;
    // finalpath = pwmdev_path + this->pwm_chip + this->pwm_channel + "/period";
    // this->pwm_period_path = pwmdev_path.c_str();

    // finalpath = pwmdev_path + this->pwm_chip + this->pwm_channel + "/enable";
    // this->pwm_enable_path = pwmdev_path.c_str();

    // finalpath = pwmdev_path + this->pwm_chip + this->pwm_channel + "/polarity";
    // this->pwm_polarity_path = pwmdev_path.c_str();

    // finalpath = pwmdev_path + this->pwm_chip + "/export";
    // this->pwm_export_path = pwmdev_path.c_str();
    

    if (this->obj == soc)
    {
        this->objtemp_path = soc_temp_path;
    }
    // else if(this->obj==ssd)
    // {
    //     this->objtemp_path=ssd_temp_path;
    // }
    writefile(pwm_enable_path, "1");
    writefile(pwm_dutycycle_path, to_string(this->fan_pwm));
    writefile(pwm_period_path, to_string(this->pwm_period));
    writefile(pwm_export_path, "0");
    writefile(pwm_polarity_path, "normal");
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
    this->obj_temp = stof(readfile(this->objtemp_path)) / 1000.f;
    switch (this->fan_mod)
    {
    case 0x01:
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

    case 0x11:
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
    socfan.init(soc_fan_configfile_path);
    ssdfan.init(ssd_fan_configfile_path);
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
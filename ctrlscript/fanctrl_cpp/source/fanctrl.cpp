#include "fanctrl.hpp"

using namespace std;

void writefile(const char *path, string data)
{
    ofstream file;
    file.open(path, ios::out);
    file << data;
    file.close();
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
            }
            else if (item == "pwm_channel")
            {
                begin = line.find_first_of('"');
                end = line.find_last_of('"');
                this->pwm_channel = line.substr(begin + 1, end - begin - 1).c_str();
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
            else if (item == "fan_pwm")
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
    string finalpath;
    string pwmdev_path = pwm_path;
    finalpath = pwmdev_path + this->pwm_chip + this->pwm_channel + "/duty_cycle";
    this->pwm_dutycycle_path = pwmdev_path.c_str();

    finalpath = pwmdev_path + this->pwm_chip + this->pwm_channel + "/period";
    this->pwm_period_path = pwmdev_path.c_str();

    finalpath = pwmdev_path + this->pwm_chip + this->pwm_channel + "/enable";
    this->pwm_enable_path = pwmdev_path.c_str();

    finalpath = pwmdev_path + this->pwm_chip + this->pwm_channel + "/polarity";
    this->pwm_polarity_path = pwmdev_path.c_str();

    finalpath = pwmdev_path + this->pwm_chip + "/export";
    this->pwm_export_path = pwmdev_path.c_str();

    string data;
    data = to_string(this->fan_pwm);
    writefile(pwm_dutycycle_path, data);

    data = to_string(this->pwm_period);
    writefile(pwm_period_path, data);
    writefile(pwm_enable_path, "0");
    writefile(pwm_export_path, "1");
    writefile(pwm_polarity_path, "normal");
}

int main()
{
    FAN socfan, ssdfan;
    socfan.init(soc_fan_configfile_path);
    ssdfan.init(ssd_fan_configfile_path);
    while (1)
    {
        usleep(1000 * 20);
    }
}
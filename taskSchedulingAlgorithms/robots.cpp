#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <iomanip> 
#include <deque>
#include <map>
#include <set>

struct Wheelbarrow {
    int number;
    std::string stone_type;
    int weight;
    int quantity;

    int loadingTime() const 
    {
        return weight * quantity;
    }
};

struct Arrival {
    int minute;
    std::vector<Wheelbarrow> wheelbarrows;
};

void load_data_from_file(const std::string &filename, std::vector<Arrival> &arrivals) 
{
    std::ifstream file(filename);
    if (!file.is_open()) 
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);
        Arrival arrival;
        iss >> arrival.minute;

        while (!iss.eof()) 
        {
            Wheelbarrow wheelbarrow;
            iss >> wheelbarrow.number >> wheelbarrow.stone_type >> wheelbarrow.weight >> wheelbarrow.quantity;
            if (!iss.fail()) 
            {
                arrival.wheelbarrows.push_back(wheelbarrow);
            }
        }
        arrivals.push_back(arrival);
    }
    file.close();
}

void load_data_from_stdin(std::vector<Arrival> &arrivals) 
{
    std::cout << "Enter data (Ctrl+D to end input):\n";
    std::string line;
    while (std::getline(std::cin, line)) 
    {
        std::istringstream iss(line);
        Arrival arrival;
        iss >> arrival.minute;

        while (!iss.eof()) 
        {
            Wheelbarrow wheelbarrow;
            iss >> wheelbarrow.number >> wheelbarrow.stone_type >> wheelbarrow.weight >> wheelbarrow.quantity;
            if (!iss.fail()) 
            {
                arrival.wheelbarrows.push_back(wheelbarrow);
            }
        }
        arrivals.push_back(arrival);
    }
}


void display_arrivals(const std::vector<Arrival> &arrivals)
{
    for (const auto &arrival : arrivals) 
    {
        std::cout << "Minute: " << arrival.minute << std::endl;
        for (const auto &wheelbarrow : arrival.wheelbarrows) 
        {
            std::cout << "  Wheelbarrow no. " << wheelbarrow.number << ", Stone type: " << wheelbarrow.stone_type << ", Weight: " << wheelbarrow.weight << ", Quantity: " << wheelbarrow.quantity << ", Loading time: " << wheelbarrow.loadingTime() << std::endl;
        }
    }
}

bool is_any_robot_busy(const std::vector<int> &robot_timers) 
{
    for (int timer : robot_timers) 
    {
        if (timer > 0) 
        {
            return true;  
        }
    }
    return false;  // no robot is busy
}

void display_robots(const std::vector<int> &robot_timers, const std::vector<std::string> &robot_tasks, int robot_count) 
{
    std::cout << std::setw(16) << " ";
        for (int i = 0; i < robot_count; i++) 
        {
            if (robot_timers[i] > 0) 
            {
                std::cout << "[" << std::left << std::setw(12) << robot_tasks[i] << robot_timers[i] << "]";
            } 
            else 
            {
                std::cout << "[" << std::setw(13) << " " <<  "]";
            }
        }
        std::cout << "\n";
}

void simulate_FCFS(const std::vector<Arrival>& arrivals, int robot_count) 
{
    std::queue<Wheelbarrow> task_queue;
    std::vector<int> robot_timers(robot_count, 0); 
    std::vector<std::string> robot_tasks(robot_count, "");

    int current_moment = 0;
    int arrival_index = 0;

    std::cout << std::endl;
    std::cout << robot_count << " robots in the mine\n" << std::endl;
    std::cout << "FCFS: \n" << std::endl;

    while (arrival_index < arrivals.size() || is_any_robot_busy(robot_timers) || !task_queue.empty()) 
    {

        bool is_new_moment = false;

        // add new arrivals to the queue in the current moment
        if (arrival_index < arrivals.size() && arrivals[arrival_index].minute == current_moment) 
        {
            if (!is_new_moment) 
            {
                std::cout << "Moment " << current_moment << "\n";
                is_new_moment = true;
            }
            for (const auto &wheelbarrow : arrivals[arrival_index].wheelbarrows) 
            {
                task_queue.push(wheelbarrow);
                std::cout << "        Wheelbarrow arrived <" << wheelbarrow.number << " " << wheelbarrow.stone_type << " " << wheelbarrow.weight << " " << wheelbarrow.quantity << " [" << wheelbarrow.loadingTime() << "]>\n";
            }
            arrival_index++;
        }

        // assign wheelbarrows to idle robots
        for (int i = 0; i < robot_count; i++) 
        {
            if (robot_timers[i] == 0 && !task_queue.empty()) 
            {
                Wheelbarrow tmp = task_queue.front(); //assign the first wheelbarrow from the queue
                robot_timers[i] = tmp.loadingTime();
                robot_tasks[i] = tmp.stone_type;
                task_queue.pop(); //remove the assigned wheelbarrow 
            }
        }

        // display the state of the robots
        if (!is_new_moment) 
            std::cout << "Moment " << current_moment << "\n";
        display_robots(robot_timers, robot_tasks, robot_count);
        
        // decrement the timers of the busy robots and reset the tasks 
        for (int i = 0; i < robot_count; i++) 
        {
            if (robot_timers[i] > 0) 
            {
                robot_timers[i]--;
                if (robot_timers[i] == 0) 
                {
                    robot_tasks[i] = "";
                }
            }
        }

        current_moment++;  // next moment
    }
}


void simulate_SRTF(const std::vector<Arrival> &arrivals, int robot_count) 
{
    std::vector<std::pair<int, Wheelbarrow>> task_queue; // remaining time and wheelbarrow
    std::vector<int> robot_timers(robot_count, 0);
    std::vector<std::string> robot_tasks(robot_count, "");
    std::vector<Wheelbarrow> robot_handled_wb(robot_count);

    int current_moment = 0;
    int arrival_index = 0;

    std::cout << "\n" << robot_count << " robots in the mine\n\n";
    std::cout << "SRTF Simulation:\n\n";

    while (arrival_index < arrivals.size() || !task_queue.empty() || is_any_robot_busy(robot_timers)) 
    {
        bool is_new_moment = false;

        // add new arrivals to the queue in the current moment
        if (arrival_index < arrivals.size() && arrivals[arrival_index].minute == current_moment) 
        {
            if (!is_new_moment) 
            {
                std::cout << "Moment " << current_moment << "\n";
                is_new_moment = true;
            }
            for (const auto& wheelbarrow : arrivals[arrival_index].wheelbarrows) 
            {
                task_queue.push_back({wheelbarrow.loadingTime(), wheelbarrow});
                std::cout << "        Wheelbarrow arrived <" << wheelbarrow.number << " "  << wheelbarrow.stone_type << " " << wheelbarrow.weight << " "  << wheelbarrow.quantity << " [" << wheelbarrow.loadingTime() << "]>\n";
            }
            arrival_index++;
        }

        // sort queue by the remaining time
        std::sort(task_queue.begin(), task_queue.end(), [](const auto &a, const auto &b) 
        {
            return a.first < b.first; 
        });

        // interrupt running tasks if necessary
        for (int i = 0; i < robot_count; i++) 
        {
            if (robot_timers[i] > 0 && !task_queue.empty() && task_queue.front().first < robot_timers[i]) 
            {
                task_queue.push_back({robot_timers[i], robot_handled_wb[i]});
                robot_timers[i] = 0;
                robot_tasks[i] = "";
            }
        }

        // assign wheelbarrows to idle robots
        for (int i = 0; i < robot_count; i++) 
        {
            if (robot_timers[i] == 0 && !task_queue.empty()) 
            {
                auto [remaining_time, wheelbarrow] = task_queue.front();
                robot_timers[i] = remaining_time;
                robot_tasks[i] = wheelbarrow.stone_type;
                robot_handled_wb[i] = wheelbarrow;
                task_queue.erase(task_queue.begin()); // remove first element from the queue
            }
        }

        // display the state of robots
        if (!is_new_moment) 
        {
            std::cout << "Moment " << current_moment << "\n";
        }
        display_robots(robot_timers, robot_tasks, robot_count);

        // decrement the timers of busy robots
        for (int i = 0; i < robot_count; i++) 
        {
            if (robot_timers[i] > 0) 
            {
                robot_timers[i]--;
                if (robot_timers[i] == 0) 
                {
                    robot_tasks[i] = "";
                }
            }
        }

        current_moment++; // next moment
    }
}

int main(int argc, char* argv[]) 
{
    if (argc != 5) 
    {
        std::cerr << "Usage: " << argv[0] << " <robots_count> <time_quantum> <filename> <scheduling_algorithm (1) or (2)>" << std::endl;
        return 1;
    }

    int robots_count = std::stoi(argv[1]);  
    int time_quantum = std::stoi(argv[2]);  
    std::string filename = argv[3];  
    int algorithm = std::stoi(argv[4]);  //1 for FCFS, 2 for RR

    std::vector<Arrival> arrivals;

    if(filename == "-")
        load_data_from_stdin(arrivals);
    else 
        load_data_from_file(filename, arrivals);
    
   
    if(algorithm == 1)
        simulate_FCFS(arrivals, robots_count);
    else if (algorithm == 2)
        simulate_SRTF(arrivals, robots_count);
    else
        std::cerr << "Invalid scheduling algorithm: (1) - RR, (2) - SRTF" << algorithm << std::endl;
       
    return 0;
}

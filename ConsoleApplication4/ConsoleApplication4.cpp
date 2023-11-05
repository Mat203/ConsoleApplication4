#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>



class Airplane {
private:
    std::string flight_number;
    std::string date;
    std::vector<std::vector<bool>> seats;
    std::map<int, double> row_prices;
public:
    Airplane(std::string fn, std::string dt, int num_rows, int num_seats_per_row)
        : flight_number(fn), date(dt) {
        seats = std::vector<std::vector<bool>>(num_rows, std::vector<bool>(num_seats_per_row, true));
        for (int i = 0; i < num_rows; i++) {
            row_prices[i] = 100.0;
        }
    }
    bool is_seat_available(int row, int seat) {
        return seats[row][seat];
    }
    void book_seat(int row, int seat) {
        if (is_seat_available(row, seat)) {
            seats[row][seat] = false;
        }
        else {
            std::cout << "This seat is booked" << std::endl;
        }
    }

    void print_flight_number() const {
        std::cout << flight_number << std::endl;
    }

    void print_date() const {
        std::cout << date << std::endl;
    }


};


class Ticket {
private:
    int ticket_id;
    std::string passenger_name;
    int seat_number;
    std::string flight_info;
public:
    Ticket() {} 
    Ticket(int id, std::string pn, int sn, std::string fi)
        : ticket_id(id), passenger_name(pn), seat_number(sn), flight_info(fi) {}
    void print_ticket() {
        std::cout << ticket_id << passenger_name << flight_info << seat_number << std::endl;
    }
};

class User {
private:
    std::string nickname;
    std::map<int, Ticket> tickets;
public:
    User(std::string nn) : nickname(nn) {}
    void book_ticket(Airplane& airplane, int row, int seat, std::string flight_info, int ticket_id) {
        if (airplane.is_seat_available(row, seat)) {
            airplane.book_seat(row, seat);
            Ticket ticket(ticket_id, nickname, seat, flight_info);
            tickets[ticket_id] = ticket;
        }
        else {
            std::cout << "This seat is already booked!" << std::endl;
        }
    }
    void print_tickets() {
        for (auto& ticket : tickets) {
            ticket.second.print_ticket();
        }
    }
};

class ConfigReader {
private:
    std::string filename;
public:
    ConfigReader(std::string fn) : filename(fn) {}
    std::vector<Airplane> readConfig() {
        std::vector<Airplane> airplanes;
        std::ifstream file(filename);
        std::string line;
        if (file.is_open()) {
            int num_records;
            if (std::getline(file, line)) {
                num_records = std::stoi(line);
            }
            for (int i = 0; i < num_records; i++) {
                if (std::getline(file, line)) {
                    std::istringstream iss(line);
                    std::string date, flight_number;
                    int num_seats_per_row = 6; 
                    int num_rows = 10;
                    iss >> date >> flight_number;
                    Airplane airplane(flight_number, date, num_rows, num_seats_per_row);
                    airplanes.push_back(airplane);
                }
            }
            file.close();
        }
        else {
            std::cout << "Unable to open file" << std::endl;
        }
        return airplanes;
    }
};

int main() {
    ConfigReader config_reader("config.txt");
    std::vector<Airplane> airplanes = config_reader.readConfig();

    for (const Airplane& airplane : airplanes) {
        airplane.print_flight_number();
        airplane.print_date();
    }

    return 0;
}


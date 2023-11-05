#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>



class Airplane {
public:
    std::string flight_number;
    std::string date;
    std::vector<std::vector<bool>> seats;
    std::map<int, double> row_prices;
    Airplane(std::string fn, std::string dt, int num_rows, int num_seats_per_row)
        : flight_number(fn), date(dt) {
        seats = std::vector<std::vector<bool>>(num_rows, std::vector<bool>(num_seats_per_row, true));
    }

    void set_row_price(int startRow, int endRow, double price) {
        for (int i = startRow; i <= endRow; ++i) {
            row_prices[i - 1] = price; 
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

    void print_info() const {
        std::cout << "Flight number: " << flight_number << std::endl;
        std::cout << "Date: " << date << std::endl;
        std::cout << "Seats:" << std::endl;
        for (int i = 0; i < seats.size(); i++) {
            for (int j = 0; j < seats[i].size(); j++) {
                std::cout << (seats[i][j] ? "Available" : "Booked") << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Row prices:" << std::endl;
        for (const auto& pair : row_prices) {
            std::cout << "Row " << pair.first + 1 << ": " << pair.second << "$" << std::endl;
        }
    }

    void check() const {
        std::cout << "Available seats for flight " << flight_number << " on " << date << ":" << std::endl;
        for (int i = 0; i < seats.size(); ++i) {
            for (int j = 0; j < seats[i].size(); ++j) {
                if (seats[i][j]) {
                    char seatLetter = 'A' + j;
                    std::cout << i + 1 << seatLetter << ": " << row_prices.at(i) << "$" << std::endl;
                }
            }
        }
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
                    int num_seats_per_row;
                    iss >> date >> flight_number >> num_seats_per_row;
                    std::vector<int> startRows, endRows, prices;
                    while (!iss.eof()) {
                        int startRow, endRow, price;
                        char dash, dollar;
                        if (!(iss >> startRow >> dash >> endRow >> price >> dollar)) {
                            break;
                        }
                        startRows.push_back(startRow);
                        endRows.push_back(endRow);
                        prices.push_back(price);
                    }
                    int num_rows = endRows.back();
                    Airplane airplane(flight_number, date, num_rows, num_seats_per_row);
                    for (size_t i = 0; i < startRows.size(); ++i) {
                        airplane.set_row_price(startRows[i], endRows[i], prices[i]);
                    }
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

void check(const std::vector<Airplane>& airplanes, const std::string& date, const std::string& flightNumber) {
    for (const auto& airplane : airplanes) {
        if (airplane.date == date && airplane.flight_number == flightNumber) {
            airplane.check();
            return;
        }
    }
    std::cout << "No flight found with date " << date << " and flight number " << flightNumber << std::endl;
}

int main() {
    ConfigReader config_reader("config.txt");
    std::vector<Airplane> airplanes = config_reader.readConfig();

    for (const Airplane& airplane : airplanes) {
        airplane.print_info();
    }

    check(airplanes, "11.12.2022", "FQ12");

    return 0;
}


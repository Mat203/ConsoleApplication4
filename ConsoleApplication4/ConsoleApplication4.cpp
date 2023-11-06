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
    int get_price(int row) {
        return row_prices[row];
    }


    void free_seat(int row, int seat) {
        seats[row][seat] = true;
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
    int row;
    int seat;
    double price;
public:
    Ticket() {}
    Ticket(int id, std::string pn, int row, int seat, double p)
        : ticket_id(id), passenger_name(pn), row(row), seat(seat), price(p) {}
    void print_ticket() {
        std::cout << "Ticket ID: " << ticket_id << "\nPassenger Name: " << passenger_name << "\nRow: " << row << "\nSeat: " << seat << "\nPrice: " << price << "$" << std::endl;
    }
    int get_row() const {
        return row;
    }
    int get_seat() const {
        return seat;
    }
    double get_price() const {
        return price;
    }
};


class User {
private:
    std::string nickname;
    std::map<int, Ticket> tickets;
public:
    User() {}
    User(std::string nn) : nickname(nn) {}
    void book_ticket(Airplane& airplane, int row, int seat, std::string flight_info, int ticket_id) {
        double price = airplane.get_price(row);
        Ticket ticket(ticket_id, nickname, row, seat, price);
        tickets[ticket_id] = ticket;
    }

    void print_tickets() {
        for (auto& ticket : tickets) {
            ticket.second.print_ticket();
        }
    }

    bool has_ticket(int ticket_id) const {
        return tickets.find(ticket_id) != tickets.end();
    }

    void return_ticket(Airplane& airplane, int ticket_id) {
        if (tickets.find(ticket_id) != tickets.end()) {
            int row = tickets[ticket_id].get_row();
            int seat = tickets[ticket_id].get_seat();
            airplane.free_seat(row, seat);
            double refund = tickets[ticket_id].get_price();
            tickets.erase(ticket_id);
            std::cout << "Refund: " << refund << "$" << std::endl;
        }
        else {
            std::cout << "Ticket not found!" << std::endl;
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

class Airline {
private:
    std::vector<Airplane> airplanes;
    std::map<std::string, User> users;
public:
    Airline(const std::string& configFilename) {
        ConfigReader config_reader(configFilename);
        airplanes = config_reader.readConfig();
    }

    void print_info() const {
        for (const Airplane& airplane : airplanes) {
            airplane.print_info();
        }
    }

    void check(const std::string& date, const std::string& flightNumber) const {
        for (const auto& airplane : airplanes) {
            if (airplane.date == date && airplane.flight_number == flightNumber) {
                airplane.check();
                return;
            }
        }
        std::cout << "No flight found with date " << date << " and flight number " << flightNumber << std::endl;
    }

    void book(const std::string& date, const std::string& flightNumber, int row, int seat, const std::string& username) {
        std::srand(std::time(0));
        for (auto& airplane : airplanes) {
            if (airplane.date == date && airplane.flight_number == flightNumber) {
                if (airplane.is_seat_available(row, seat)) {
                    int id1 = std::rand() % 10 + 10;
                    int id2 = std::rand() % 100 + 100;
                    int ticket_id = id1 * 1000 + id2;
                    airplane.book_seat(row, seat);
                    std::string flight_info = date + " " + flightNumber;
                    users[username].book_ticket(airplane, row, seat, flight_info, ticket_id);
                    std::cout << "Confirmed with ID " << ticket_id << std::endl;
                }
                else {
                    std::cout << "This seat is already booked!" << std::endl;
                }
            }
        }
    }

    void return_ticket(int ticket_id) {
        for (auto& user_pair : users) {
            User& user = user_pair.second;
            if (user.has_ticket(ticket_id)) {
                for (auto& airplane : airplanes) {
                    user.return_ticket(airplane, ticket_id);
                    std::cout << "Ticket returned successfully." << std::endl;
                    return;
                }
            }
        }
        std::cout << "Ticket not found!" << std::endl;
    }
};

int main() {
    Airline airline("config.txt");

    while (true) {
        std::cout << "Enter the command: ";
        std::string command;
        std::cin >> command;
        airline.print_info();

        if (command == "check") {
            std::string date, flightNumber;
            std::cout << "Date: ";
            std::cin >> date;
            std::cout << "Flight number: ";
            std::cin >> flightNumber;
            airline.check(date, flightNumber);
        }
        else if (command == "book") {
            std::string date, flightNumber, username;
            int row, seat;
            std::cout << "Date: ";
            std::cin >> date;
            std::cout << "Flight number: ";
            std::cin >> flightNumber;
            std::cout << "Enter row: ";
            std::cin >> row;
            std::cout << "Enter seat: ";
            std::cin >> seat;
            std::cout << "Enter nickname: ";
            std::cin >> username;
            airline.book(date, flightNumber, row-1, seat-1, username);
        }
        else if (command == "return") {
            int ticket_id;
            std::cout << "Ticket ID: ";
            std::cin >> ticket_id;
            std::string username;
            airline.return_ticket(ticket_id);
        }
        else if (command == "quit") {
            break;
        }
        else {
            std::cout << "Unknown command!" << std::endl;
        }
    }

    return 0;
}

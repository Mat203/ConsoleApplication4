#include <iostream>
#include <string>
#include <map>
#include <vector>



class Airplane {
private:
    std::string flight_number;
    std::vector<std::vector<bool>> seats; 
    std::map<int, double> row_prices; 
public:
    Airplane(std::string fn, int num_rows, int num_seats_per_row) : flight_number(fn) {
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
};


class Ticket {
private:
    int ticket_id;
    std::string passenger_name;
    int seat_number;
    std::string flight_info;
public:
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

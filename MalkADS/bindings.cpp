#include "include/managers/BooksManager.h"
#include "include/managers/LaptopsManager.h"
#include "include/managers/RoomsManager.h"
#include "include/managers/UsersManager.h"
#include "include/models/user.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

// Python-friendly wrapper for LibrarySystem
// Uses ONLY custom data structures - NO STL in this wrapper
class PyLibraryWrapper {
private:
  UsersManager users;
  RoomsManager rooms;
  LaptopsManager laptops;
  BooksManager books;

public:
  PyLibraryWrapper() {
    users.loadUsersFromFile();
    rooms.syncUserBookings(users);
    laptops.syncUserBookings(users);
    books.syncUserBookings(users);
  }

  ~PyLibraryWrapper() {
    // Ensure data is saved when object is destroyed
    saveAll();
  }

  void saveAll() {
    // Manually trigger saves to ensure data persists
    rooms.saveRoomsToFile();
    rooms.saveRoomBookingsToFile();
    laptops.saveLaptopsToFile();
    laptops.saveLaptopBookingsToFile();
    books.saveBooksToFile();
    books.saveBookBookingsToFile();
  }

  // Login - returns dict with success, username, is_admin
  py::dict login(const char *username, const char *password) {
    py::dict result;

    User *loggedIn = users.login(username, password);
    if (!loggedIn) {
      result["success"] = false;
      return result;
    }

    result["success"] = true;
    result["username"] = loggedIn->getUsername().c_str();
    result["is_admin"] = loggedIn->getIsAdmin();

    return result;
  }

  // Get list of rooms
  // Get list of rooms
  py::list getRooms() {
    py::list roomsList;
    rooms.forEachRoom([&](const std::string &id) { roomsList.append(id); });
    return roomsList;
  }

  // Get bookings for a specific room
  py::list getRoomBookings(const char *roomId) {
    py::list bookingsList;
    rooms.getRoomBookings(roomId,
                          [&](int start, int end, const std::string &username) {
                            py::dict booking;
                            booking["start"] = start;
                            booking["end"] = end;
                            booking["username"] = username;
                            bookingsList.append(booking);
                          });
    return bookingsList;
  }

  // Get list of laptops
  py::list getLaptops() {
    py::list laptopsList;
    laptops.forEachLaptop(
        [&](const std::string &id) { laptopsList.append(id); });
    return laptopsList;
  }

  // Get list of books
  py::list getBooks() {
    py::list booksList;
    books.forEachBook([&](const std::string &id) { booksList.append(id); });
    return booksList;
  }

  // Get all books with details for search
  py::list searchBooks() {
    py::list booksList;
    books.forEachBookWithDetails([&](const std::string &id,
                                     const std::string &title,
                                     const std::string &author) {
      py::dict bookInfo;
      bookInfo["id"] = id;
      bookInfo["title"] = title;
      bookInfo["author"] = author;
      booksList.append(bookInfo);
    });
    return booksList;
  }

  // Book a room
  py::dict bookRoom(const char *roomId, int start, int end,
                    const char *username) {
    py::dict result;

    User *user = users.getUser(username);
    if (!user) {
      result["success"] = false;
      result["message"] = "User not found";
      return result;
    }

    bool success = rooms.bookRoomDirect(user, roomId, start, end);
    result["success"] = success;

    if (success) {
      result["message"] = "Room booked successfully!";
    } else {
      result["message"] = "Conflict detected! Room is already booked or you "
                          "have a conflicting booking.";
    }

    return result;
  }

  // Borrow a laptop (specific ID)
  bool borrowLaptop(const std::string &laptopId, int start, int end,
                    const std::string &username) {
    User *user = users.getUser(username);
    if (!user)
      return false;
    bool success = laptops.borrowLaptopDirect(user, laptopId, start, end);
    if (success)
      saveAll();
    return success;
  }

  // Borrow any available laptop
  py::dict borrowAnyLaptop(int start, int end, const std::string &username) {
    py::dict result;
    User *user = users.getUser(username);
    if (!user) {
      result["success"] = false;
      result["message"] = "User not found";
      return result;
    }

    if (!user->canBookLaptop(start, end)) {
      result["success"] = false;
      result["message"] = "Limit reached: You already have a laptop borrowed "
                          "during this period.";
      return result;
    }

    bool success = false;
    std::string bookedId = "";

    laptops.forEachLaptop([&](const std::string &id) {
      if (!success) {
        // Try to book this laptop
        if (laptops.borrowLaptopDirect(user, id, start, end)) {
          success = true;
          bookedId = id;
        }
      }
    });

    result["success"] = success;
    if (success) {
      saveAll();
      result["message"] = "Laptop " + bookedId + " assigned successfully!";
    } else {
      result["message"] = "No laptops available for the selected time.";
    }
    return result;
  }

  // Borrow a book
  py::dict borrowBook(const char *bookId, int start, int end,
                      const char *username) {
    py::dict result;

    User *user = users.getUser(username);
    if (!user) {
      result["success"] = false;
      result["message"] = "User not found";
      return result;
    }

    bool success = books.borrowBookDirect(user, bookId, start, end);
    result["success"] = success;

    if (success) {
      result["message"] = "Book borrowed successfully!";
    } else {
      result["message"] = "Conflict detected! Book is not available or you've "
                          "reached the limit of 3 concurrent books.";
    }

    return result;
  }

  // Get user bookings
  py::list getUserBookings(const char *username) {
    py::list allBookings;

    // Room bookings
    rooms.forEachBooking([&](const std::string &resourceId, int start, int end,
                             const std::string &bookedBy) {
      if (bookedBy == username) {
        py::dict booking;
        booking["type"] = "room";
        booking["resource_id"] = resourceId;
        booking["start_time"] = start;
        booking["end_time"] = end;
        allBookings.append(booking);
      }
    });

    // Laptop bookings
    laptops.forEachBooking([&](const std::string &resourceId, int start,
                               int end, const std::string &bookedBy) {
      if (bookedBy == username) {
        py::dict booking;
        booking["type"] = "laptop";
        booking["resource_id"] = resourceId;
        booking["start_time"] = start;
        booking["end_time"] = end;
        allBookings.append(booking);
      }
    });

    // Book bookings
    books.forEachBooking([&](const std::string &resourceId, int start, int end,
                             const std::string &bookedBy) {
      if (bookedBy == username) {
        py::dict booking;
        booking["type"] = "book";
        booking["resource_id"] = resourceId;
        booking["start_time"] = start;
        booking["end_time"] = end;

        // Add book details
        Book *book = books.getBook(resourceId);
        if (book) {
          booking["title"] = book->getTitle();
          booking["author"] = book->getAuthor();
        } else {
          booking["title"] = "Unknown Title";
          booking["author"] = "Unknown Author";
        }

        allBookings.append(booking);
      }
    });

    return allBookings;
  }

  // Admin functions
  void addRoom(const char *roomId) { rooms.addRoomDirect(roomId); }

  void removeRoom(const char *roomId) { rooms.removeRoomDirect(roomId); }

  void addLaptop(const char *laptopId) { laptops.addLaptopDirect(laptopId); }

  void removeLaptop(const char *laptopId) {
    laptops.removeLaptopDirect(laptopId);
  }

  void addBook(const char *bookId, const char *title, const char *author) {
    books.addBookDirect(bookId, title, author);
  }

  void removeBook(const char *bookId) { books.removeBookDirect(bookId); }
};

PYBIND11_MODULE(library_system, m) {
  m.doc() = "AUC Library System - Python bindings for C++ backend";

  // Expose the wrapper class
  py::class_<PyLibraryWrapper>(m, "LibrarySystem")
      .def(py::init<>())
      .def("login", &PyLibraryWrapper::login)
      .def("get_rooms", &PyLibraryWrapper::getRooms)
      .def("get_room_bookings", &PyLibraryWrapper::getRoomBookings)
      .def("get_laptops", &PyLibraryWrapper::getLaptops)
      .def("get_books", &PyLibraryWrapper::getBooks)
      .def("search_books", &PyLibraryWrapper::searchBooks)
      .def("book_room", &PyLibraryWrapper::bookRoom)
      .def("borrow_laptop", &PyLibraryWrapper::borrowLaptop)
      .def("borrow_any_laptop", &PyLibraryWrapper::borrowAnyLaptop)
      .def("borrow_book", &PyLibraryWrapper::borrowBook)
      .def("get_user_bookings", &PyLibraryWrapper::getUserBookings)
      .def("add_room", &PyLibraryWrapper::addRoom)
      .def("remove_room", &PyLibraryWrapper::removeRoom)
      .def("add_laptop", &PyLibraryWrapper::addLaptop)
      .def("remove_laptop", &PyLibraryWrapper::removeLaptop)
      .def("add_book", &PyLibraryWrapper::addBook)
      .def("remove_book", &PyLibraryWrapper::removeBook)
      .def("save", &PyLibraryWrapper::saveAll); // Explicit save method
}

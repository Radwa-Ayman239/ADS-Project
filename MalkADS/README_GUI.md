# AUC Library System - GUI User Guide

## Installation and Setup

### Prerequisites
- Python 3.13.6 (already installed ✅)
- CMake 3.10 or higher
- Visual Studio or MinGW (for C++ compilation on Windows)
- pip (Python package manager)

### Step 1: Install Python Dependencies

Open PowerShell in the project directory and run:

```powershell
pip install -r requirements.txt
```

This will install:
- **pybind11** - For C++ Python bindings
- **customtkinter** - Modern GUI framework
- **Pillow** - Image support

### Step 2: Build the C++ Extension

#### Option A: Using CMake (Recommended)

```powershell
# Navigate to cmake build directory
cd cmake-build-debug

# Configure CMake (this will detect Python and pybind11)
cmake ..

# Build the project
cmake --build .
```

This will create `library_system.pyd` (Python extension module) in the project root.

#### Option B: If CMake Build Fails

If pybind11 is not found by CMake, install it globally:

```powershell
pip install "pybind11[global]"
```

Then retry the CMake build.

### Step 3: Run the GUI Application

```powershell
# From the project root directory
python gui/main.py
```

## Using the Application

### Login Screen

- **Username**: Enter your credentials
- **Password**: Enter your password

**Test Accounts:**
- Regular User: `user1` / `pass1`
- Administrator: `admin` / `admin123`

### User Dashboard

Once logged in as a regular user, you can:

1. **Book a Room** 🏛️
   - Select a room from available options
   - Choose start and end times (24-hour format)
   - System checks for conflicts automatically

2. **Borrow a Laptop** 💻
   - Select a laptop
   - Choose booking period
   - Conflict detection ensures availability

3. **Borrow a Book** 📚
   - Select a book
   - Choose borrowing period
   - Limited to 3 concurrent book borrowings

4. **View My Bookings** 📋
   - See all your current reservations
   - Organized by category (rooms, laptops, books)

### Admin Dashboard

Administrators have additional capabilities:

**Bookings Tab:**
- All regular user booking functions

**Management Tab:**
- **Add Book** - Add new books to the library
- **Remove Book** - Remove existing books
- **Add Laptop** - Add new laptops
- **Remove Laptop** - Remove existing laptops
- **Add Room** - Add new study rooms
- **Remove Room** - Remove existing rooms

## Features

### Time Conflict Detection
The system uses **Red-Black Interval Trees** (custom C++ data structure) to efficiently detect booking conflicts:
- Prevents double-booking of resources
- Ensures users don't have overlapping reservations
- O(log n) conflict detection performance

### Data Persistence
All bookings and resources are automatically saved to text files in the `data/` directory:
- `users.txt` - User accounts
- `rooms.txt` - Available rooms
- `laptops.txt` - Available laptops
- `books.txt` - Available books
- `room_bookings.txt` - Room reservations
- `laptop_bookings.txt` - Laptop borrowings
- `book_bookings.txt` - Book borrowings

### Modern GUI Design
- **Dark theme** with vibrant accent colors
- **Card-based layouts** for intuitive navigation
- **Real-time validation** and error feedback
- **Modal dialogs** for booking and management
- **Responsive design** that works at different window sizes

## Troubleshooting

### "library_system module not found"
The GUI will run in development/demo mode if the C++ module isn't built. To fix:
1. Ensure pybind11 is installed: `pip install pybind11`
2. Rebuild the CMake project
3. Check that `library_system.pyd` exists in the project root

### "CMake cannot find pybind11"
Install pybind11 globally:
```powershell
pip install "pybind11[global]"
```

### "No resources available"
Make sure the data files in the `data/` directory have content:
- Add rooms to `rooms.txt` (e.g., R101, R102)
- Add laptops to `laptops.txt` (e.g., L001, L002)
- Add books to `books.txt` (e.g., B0001, B0002)

### Build Errors on Windows
Ensure you have Visual Studio Build Tools installed:
- Download from: https://visualstudio.microsoft.com/downloads/
- Select "Desktop development with C++" workload

## Technical Architecture

### C++ Backend
- **HashMap** - Custom hash table for O(1) resource lookups
- **Red-Black Interval Tree** - Balanced BST for conflict detection
- **Priority Queue** - Custom heap implementation
- **LibrarySystem** - Main orchestration class

### Python GUI
- **CustomTkinter** - Modern themed widgets
- **Component Architecture** - Modular screen design
- **State Management** - Centralized user session handling
- **pybind11 Bindings** - Direct C++ function calls from Python

### Data Flow
1. User interacts with Python GUI
2. GUI calls C++ functions via pybind11
3. C++ performs conflict detection using interval trees
4. Results returned to Python
5. GUI updates with success/error messages

## Development Mode

The GUI can run without the C++ module for interface development:
- Uses mock data for resources
- Simulates booking operations
- All UI features work normally
- Useful for testing and design iterations

To enable this, simply run the GUI without building the C++ extension.

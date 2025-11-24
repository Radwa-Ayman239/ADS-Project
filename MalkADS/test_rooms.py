import sys
import os

# Add current directory to path
sys.path.append(os.getcwd())

try:
    import library_system
    lib = library_system.LibrarySystem()
    
    print("Testing getRooms...")
    rooms = lib.get_rooms()
    print(f"Rooms found: {rooms}")
    
    if not rooms:
        print("ERROR: No rooms found!")
    else:
        print(f"Testing getRoomBookings for {rooms[0]}...")
        bookings = lib.get_room_bookings(rooms[0])
        print(f"Bookings: {bookings}")
        
except ImportError as e:
    print(f"ImportError: {e}")
except Exception as e:
    print(f"Error: {e}")

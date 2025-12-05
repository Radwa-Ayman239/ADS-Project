import datetime
import time


def get_current_year():
    """Get the fixed reference year"""
    return 2025


def get_start_of_year_timestamp():
    """Get the timestamp for the start of the reference year (2025)"""
    year = get_current_year()
    start_of_year = datetime.datetime(year, 1, 1, 0, 0, 0)
    return int(start_of_year.timestamp())


def datetime_to_seconds(day, month, year, hour, minute):
    """Convert date and time components to seconds from start of the year"""
    try:
        dt = datetime.datetime(year, month, day, hour, minute)
        start_of_year = datetime.datetime(year, 1, 1, 0, 0, 0)

        # Calculate difference in seconds
        delta = dt - start_of_year
        return int(delta.total_seconds())
    except ValueError:
        return None


def seconds_to_datetime_string(seconds):
    """Convert seconds from start of year to formatted string"""
    try:
        year = get_current_year()
        start_of_year = datetime.datetime(year, 1, 1, 0, 0, 0)

        dt = start_of_year + datetime.timedelta(seconds=seconds)
        return dt.strftime("%d/%m/%Y %H:%M")
    except (ValueError, OSError, OverflowError):
        return "Invalid Date"


def get_seconds_from_start_of_year_for_date(day, month, year):
    """Get the seconds offset for the start of a specific day (00:00)"""
    return datetime_to_seconds(day, month, year, 0, 0)

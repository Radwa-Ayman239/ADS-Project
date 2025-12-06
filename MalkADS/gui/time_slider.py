import customtkinter as ctk
import tkinter as tk
from styles import COLORS, FONTS


class TimeSlider(ctk.CTkFrame):
    """
    A specific timeline slider for selecting a time range (Start -> End)
    within a 24-hour day (0 - 1440 minutes).
    Visualize available times in Green, booked (and disabled) times in Red.
    Allows dragging two flags (handles) to select range.
    """

    def __init__(self, master, width=600, height=80, on_change=None, **kwargs):
        super().__init__(master, **kwargs)
        self.canvas_width = width
        self.canvas_height = height
        self.on_change = on_change

        # Helper to handle color tuples (default to dark mode color at index 1)
        self.get_color = lambda c: c[1] if isinstance(c, (tuple, list)) else c

        # Data (minutes 0-1440)
        self.total_minutes = 24 * 60
        self.blocked_intervals = []  # list of (start_min, end_min)
        self.disabled_until = 0  # Minutes from start of day to render as red/disabled

        # Selection state (minutes)
        self.start_min = 9 * 60  # Default 09:00
        self.end_min = 10 * 60  # Default 10:00

        # UI configuration
        self.margin_x = 20
        self.track_y = 40
        self.track_height = 20
        self.handle_radius = 8
        self.track_width = self.canvas_width - 2 * self.margin_x

        # Canvas
        self.canvas = ctk.CTkCanvas(
            self,
            width=self.canvas_width,
            height=self.canvas_height,
            bg=self.get_color(COLORS["bg_card"]),
            highlightthickness=0,
        )
        self.canvas.pack(fill="both", expand=True)

        # Bind events
        self.canvas.bind("<Button-1>", self.on_click)
        self.canvas.bind("<B1-Motion>", self.on_drag)
        self.canvas.bind("<ButtonRelease-1>", self.on_release)

        self.dragging_handle = None

        # Dynamic Resizing
        self.bind("<Configure>", self.on_resize)

        self.draw()

    def on_resize(self, event):
        self.canvas_width = event.width
        self.track_width = self.canvas_width - 2 * self.margin_x
        self.draw()

    def set_blocked_intervals(self, intervals):
        """
        Set booked intervals to display as red.
        intervals: list of dict {'start': min, 'end': min} or tuples
        """
        self.blocked_intervals = []
        for i in intervals:
            if isinstance(i, dict):
                self.blocked_intervals.append((i["start"], i["end"]))
            else:
                self.blocked_intervals.append(i)
        self.draw()

    def set_disabled_until(self, minutes):
        """Set a time (in minutes) before which everything is red (e.g. past time)"""
        self.disabled_until = minutes
        self.draw()

    def min_to_x(self, m):
        return self.margin_x + (m / self.total_minutes) * self.track_width

    def x_to_min(self, x):
        m = ((x - self.margin_x) / self.track_width) * self.total_minutes
        return max(0, min(self.total_minutes, int(m)))

    def format_time(self, m):
        h = int(m // 60)
        mn = int(m % 60)
        return f"{h:02d}:{mn:02d}"

    def draw(self):
        self.canvas.delete("all")

        # 1. Base Green Track (Available)
        self.canvas.create_rectangle(
            self.margin_x,
            self.track_y,
            self.margin_x + self.track_width,
            self.track_y + self.track_height,
            fill=self.get_color(COLORS["success"]),
            outline="",
            width=0,
        )

        # 2a. Disabled/Past Block (Red)
        if self.disabled_until > 0:
            x_past = self.min_to_x(min(self.total_minutes, self.disabled_until))
            self.canvas.create_rectangle(
                self.margin_x,
                self.track_y,
                x_past,
                self.track_y + self.track_height,
                fill=self.get_color(COLORS["error"]),
                outline="",
            )

        # 2b. Red Blocks (Booked)
        for start, end in self.blocked_intervals:
            x1 = self.min_to_x(max(0, start))
            x2 = self.min_to_x(min(self.total_minutes, end))
            if x2 > x1:
                self.canvas.create_rectangle(
                    x1,
                    self.track_y,
                    x2,
                    self.track_y + self.track_height,
                    fill=self.get_color(COLORS["error"]),
                    outline="",
                )

        # 3. Selected Range Overlay (Blue semi-transparent?)
        sx = self.min_to_x(self.start_min)
        ex = self.min_to_x(self.end_min)

        # Highlight selected part on lower half of track or just center line
        self.canvas.create_rectangle(
            sx,
            self.track_y + 5,
            ex,
            self.track_y + self.track_height - 5,
            fill=self.get_color(COLORS["primary"]),
            stipple="gray50",
            outline="",  # stipple works on windows usually
        )

        # 4. Handles (Flags)
        # Start Handle
        self.draw_handle(sx, "start", self.start_min, top=False)
        # End Handle
        self.draw_handle(ex, "end", self.end_min, top=True)

        # 5. Hour markers text (optional, e.g. every 6 hours)
        for h in [0, 6, 12, 18, 24]:
            mx = self.min_to_x(h * 60)
            self.canvas.create_text(
                mx,
                self.track_y + self.track_height + 15,
                text=f"{h:02d}:00",
                fill="gray",
                font=("Arial", 8),
            )

    def draw_handle(self, x, tag, time_val, top=True):
        y = self.track_y + self.track_height // 2
        r = self.handle_radius

        color = self.get_color(COLORS["text_primary"])

        # Draw vertical line
        self.canvas.create_line(
            x,
            self.track_y - 5,
            x,
            self.track_y + self.track_height + 5,
            fill=color,
            width=2,
        )

        # Draw Circle/Knob
        self.canvas.create_oval(
            x - r, y - r, x + r, y + r, fill="#FFFFFF", outline=color, width=2, tags=tag
        )

        # Time Label
        label_y = (
            self.track_y - 15 if top else self.track_y + self.track_height + 25
        )  # alternate positions
        if not top:
            label_y = (
                self.track_y - 15
            )  # Actually let's put both on top to avoid overlap with axis

        # Adjust y offset for start v end overlap prevention
        if tag == "start":
            label_y -= 10

        text_id = self.canvas.create_text(
            x,
            label_y,
            text=self.format_time(time_val),
            fill=color,
            font=("Arial", 10, "bold"),
        )

        # Add background to text for readability
        bbox = self.canvas.bbox(text_id)
        # Expand bbox
        bbox = (bbox[0] - 2, bbox[1] - 1, bbox[2] + 2, bbox[3] + 1)
        bg_id = self.canvas.create_rectangle(
            bbox, fill=self.get_color(COLORS["bg_card"]), outline="", tags="label_bg"
        )
        self.canvas.tag_lower(bg_id, text_id)

    def on_click(self, event):
        x = event.x
        y = event.y

        # Check distance to handles based on x
        sx = self.min_to_x(self.start_min)
        ex = self.min_to_x(self.end_min)

        dist_s = abs(x - sx)
        dist_e = abs(x - ex)

        threshold = 20  # pixels

        if dist_s < threshold and dist_s < dist_e:
            self.dragging_handle = "start"
        elif dist_e < threshold:
            self.dragging_handle = "end"
        else:
            # Maybe click to jump closest handle?
            if dist_s < dist_e:
                self.start_min = self.x_to_min(x)
                if self.start_min >= self.end_min:
                    self.start_min = self.end_min - 10
                self.dragging_handle = "start"
            else:
                self.end_min = self.x_to_min(x)
                if self.end_min <= self.start_min:
                    self.end_min = self.start_min + 10
                self.dragging_handle = "end"
            self.draw()
            self.trigger_callback()

    def on_drag(self, event):
        if not self.dragging_handle:
            return

        m = self.x_to_min(event.x)

        if self.dragging_handle == "start":
            self.start_min = m
            # Constraint: Start < End
            if self.start_min >= self.end_min:
                self.start_min = self.end_min - 5  # Keep 5 min gap
        else:
            self.end_min = m
            # Constraint: End > Start
            if self.end_min <= self.start_min:
                self.end_min = self.start_min + 5

        self.draw()
        self.trigger_callback()

    def on_release(self, event):
        self.dragging_handle = None

    def trigger_callback(self):
        if self.on_change:
            # Pass formatted strings just for ease, or ints
            # The interface with BookingDialog expects strings or population of Entry

            # format: HH, MM
            sh = int(self.start_min // 60)
            sm = int(self.start_min % 60)
            eh = int(self.end_min // 60)
            em = int(self.end_min % 60)

            self.on_change(sh, sm, eh, em)

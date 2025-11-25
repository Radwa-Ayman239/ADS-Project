"""
Centralized styling and color themes for the GUI
"""

# Color Palette - Dual Theme (Light, Dark)
# Format: (Light Mode Color, Dark Mode Color)
COLORS = {
    # Primary colors
    "primary": ("#0984e3", "#00a8ff"),  # Slightly darker blue for light mode
    "primary_hover": ("#74b9ff", "#0097e6"),
    "primary_dark": ("#005a8d", "#005a8d"),
    # Secondary colors
    "secondary": ("#d63031", "#e84118"),  # Red
    "secondary_hover": ("#ff7675", "#c23616"),
    # Success/Error/Warning
    "success": ("#00b894", "#4cd137"),  # Green
    "success_hover": ("#55efc4", "#44bd32"),
    "error": ("#d63031", "#e84118"),  # Red
    "error_hover": ("#ff7675", "#c23616"),
    "warning": ("#fdcb6e", "#fbc531"),  # Yellow
    "warning_hover": ("#ffeaa7", "#e1b12c"),
    # Backgrounds
    "bg_dark": ("#f5f6fa", "#1e272e"),  # Main Background (Light Grey / Dark Blue-Grey)
    "bg_medium": ("#dfe6e9", "#2f3640"),  # Secondary Background
    "bg_light": ("#b2bec3", "#353b48"),  # Tertiary
    "bg_card": ("#ffffff", "#2f3640"),  # Card Background (White / Dark Grey)
    # Text
    "text_primary": ("#2d3436", "#f5f6fa"),  # Dark Grey / White
    "text_secondary": ("#636e72", "#dcdde1"),  # Grey / Light Grey
    "text_dim": ("#b2bec3", "#7f8fa6"),  # Light Grey / Dim Grey
    # Accent colors
    "accent_blue": ("#0984e3", "#00a8ff"),
    "accent_purple": ("#6c5ce7", "#9c88ff"),
    "accent_green": ("#00b894", "#4cd137"),
    "accent_orange": ("#e17055", "#fbc531"),
    "accent_cyan": ("#00cec9", "#00d2d3"),
    # Gradients (simulated)
    "gradient_start": ("#0984e3", "#00a8ff"),
    "gradient_end": ("#6c5ce7", "#9c88ff"),
}

# Font Configuration
FONTS = {
    "title": ("Roboto", 32, "bold"),
    "heading": ("Roboto", 24, "bold"),
    "subheading": ("Roboto", 18, "bold"),
    "body": ("Roboto", 14),
    "body_large": ("Roboto", 16),
    "button": ("Roboto", 14, "bold"),
    "small": ("Roboto", 12),
    "tiny": ("Roboto", 10),
}

# Spacing and sizing
SPACING = {
    "xs": 8,
    "sm": 16,
    "md": 24,
    "lg": 32,
    "xl": 48,
}

SIZES = {
    "button_height": 50,
    "input_height": 45,
    "card_corner": 20,
    "button_corner": 12,
    "input_corner": 10,
}

# Animation constants
ANIMATIONS = {
    "fast": 150,
    "medium": 300,
    "slow": 500,
    "hover_scale": 1.02,
    "slide_distance": 20,
}

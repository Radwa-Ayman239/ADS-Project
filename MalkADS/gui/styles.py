"""
Centralized styling and color themes for the GUI
"""

# Color Palette - Modern and Professional
COLORS = {
    # Primary colors
    "primary": "#1f538d",  # Deep blue
    "primary_hover": "#2d6db5",
    "primary_dark": "#14365e",
    
    # Secondary colors
    "secondary": "#e74c3c",  # Vibrant red
    "secondary_hover": "#c0392b",
    
    # Success/Error/Warning
    "success": "#27ae60",
    "success_hover": "#229954",
    "error": "#e74c3c",
    "error_hover": "#c0392b",
    "warning": "#f39c12",
    "warning_hover": "#e67e22",
    
    # Neutral colors
    "bg_dark": "#1a1a1a",
    "bg_medium": "#2b2b2b",
    "bg_light": "#3a3a3a",
    "bg_card": "#2d2d2d",
    "text_primary": "#ffffff",
    "text_secondary": "#b0b0b0",
    "text_dim": "#808080",
    
    # Accent colors
    "accent_blue": "#3498db",
    "accent_purple": "#9b59b6",
    "accent_green": "#2ecc71",
    "accent_orange": "#e67e22",
    
    # Gradients (for future use)
    "gradient_start": "#667eea",
    "gradient_end": "#764ba2",
}

# Font Configuration
FONTS = {
    "title": ("Segoe UI", 32, "bold"),
    "heading": ("Segoe UI", 24, "bold"),
    "subheading": ("Segoe UI", 18, "bold"),
    "body": ("Segoe UI", 14),
    "body_large": ("Segoe UI", 16),
    "button": ("Segoe UI", 14, "bold"),
    "small": ("Segoe UI", 12),
    "tiny": ("Segoe UI", 10),
}

# Spacing and sizing
SPACING = {
    "xs": 5,
    "sm": 10,
    "md": 20,
    "lg": 30,
    "xl": 40,
}

SIZES = {
    "button_height": 45,
    "input_height": 40,
    "card_corner": 15,
    "button_corner": 10,
    "input_corner": 8,
}

# Animation timings (in milliseconds)
ANIMATIONS = {
    "fast": 150,
    "medium": 300,
    "slow": 500,
}

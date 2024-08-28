# -*- coding: utf-8 -*-
"""
Created on Tue Aug 27 21:58:18 2024

@author: daniel carvalho
"""
def rgb_to_hsl(r, g, b):
    r /= 255.0
    g /= 255.0
    b /= 255.0

    max_color = max(r, g, b)
    min_color = min(r, g, b)
    l = (max_color + min_color) / 2.0

    if max_color == min_color:
        h = s = 0.0  # Achromatic
    else:
        delta = max_color - min_color
        s = delta / (2.0 - max_color - min_color) if l > 0.5 else delta / (max_color + min_color)

        if max_color == r:
            h = (g - b) / delta + (6.0 if g < b else 0.0)
        elif max_color == g:
            h = (b - r) / delta + 2.0
        else:
            h = (r - g) / delta + 4.0

        h /= 6.0

    h = round(h * 360, 2)  # Convert to degrees and round
    s = round(s * 100, 2)  # Convert to percentage and round
    l = round(l * 100, 2)  # Convert to percentage and round

    return h, s, l

# Example usage:
r, g, b = 255, 0, 0  # Red color
h, s, l = rgb_to_hsl(r, g, b)
print(f"H: {h}, S: {s}%, L: {l}%")



# Example usage:
r, g, b = 127, 0, 255  # Red color
h, s, l = rgb_to_hsl(r, g, b)
print(f"H: {h}, S: {s}%, L: {l}%")



# Example usage:
r, g, b = 127, 255, 0  # Red color
h, s, l = rgb_to_hsl(r, g, b)
print(f"H: {h}, S: {s}%, L: {l}%")

# Example usage:
r, g, b = 0, 0, 0  # Red color
h, s, l = rgb_to_hsl(r, g, b)
print(f"H: {h}, S: {s}%, L: {l}%")


# Example usage:
r, g, b = 255, 255, 255  # Red color
h, s, l = rgb_to_hsl(r, g, b)
print(f"H: {h}, S: {s}%, L: {l}%")
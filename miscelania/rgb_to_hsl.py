
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


def rgb_to_hsv(r, g, b):
    # Normalize the RGB values to range [0, 1]
    r, g, b = r / 255.0, g / 255.0, b / 255.0

    # Compute the minimum and maximum RGB values
    cmax = max(r, g, b)
    cmin = min(r, g, b)
    diff = cmax - cmin

    # Calculate Hue
    if cmax == cmin:
        h = 0
    elif cmax == r:
        h = (60 * ((g - b) / diff) + 360) % 360
    elif cmax == g:
        h = (60 * ((b - r) / diff) + 120) % 360
    elif cmax == b:
        h = (60 * ((r - g) / diff) + 240) % 360

    # Calculate Saturation
    if cmax == 0:
        s = 0
    else:
        s = (diff / cmax) * 100

    # Calculate Value
    v = cmax * 100

    return h, s, v

def call_f(r,g,b):
    
    print("H: {}, S: {}%, L: {}%".format(*rgb_to_hsv(r, g, b)))
    print("H: {}, S: {}%, L: {}%".format(*rgb_to_hsl(r, g, b)))


call_f(r=63,g=0,b=0)
call_f(r=127,g=0,b=255)
call_f(r=127,g=255,b=0)
call_f(r=255,g=255,b=255)
call_f(r=63,g=0,b=0)


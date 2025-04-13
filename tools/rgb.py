import readline
raw_input = input("Enter RGB values separated by spaces: ")
rgb = tuple(map(int, raw_input.strip().split()))
# Scale to [0.0, 1.0] and round to 2 decimal places
scaled_rgb = tuple(round(x / 255.0, 2) for x in rgb)
print("Scaled RGB:", scaled_rgb)

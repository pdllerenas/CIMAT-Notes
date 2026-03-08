import matplotlib.pyplot as plt
import matplotlib.patches as patches
import re

def plot_quadtree(filename):
    points = set() # Use a set to prevent plotting duplicate points
    rects = []
    
    # Regular expressions to match your specific C++ output format
    rect_pattern = re.compile(r'^\[(.*),(.*),(.*),(.*)\]$')
    point_pattern = re.compile(r'^\((.*),(.*)\)$')

    # Parse the output file
    try:
        with open(filename, 'r') as f:
            for line in f:
                line = line.strip()
                
                # Check if the line is a Rectangle
                rect_match = rect_pattern.match(line)
                if rect_match:
                    xmin, xmax, ymin, ymax = map(float, rect_match.groups())
                    rects.append((xmin, xmax, ymin, ymax))
                    continue
                
                # Check if the line is a Point
                point_match = point_pattern.match(line)
                if point_match:
                    x, y = map(float, point_match.groups())
                    points.add((x, y))
    except FileNotFoundError:
        print(f"Error: Could not find '{filename}'. Make sure you saved the C++ output!")
        return

    # Set up the plot
    fig, ax = plt.subplots(figsize=(8, 8))
    
    # 1. Draw all the Rectangles (QuadTree nodes)
    for xmin, xmax, ymin, ymax in rects:
        width = xmax - xmin
        height = ymax - ymin
        # Create a hollow rectangle with a red border
        rect = patches.Rectangle((xmin, ymin), width, height, 
                                 linewidth=1.2, edgecolor='red', facecolor='none', alpha=0.6)
        ax.add_patch(rect)

    # 2. Draw all the Points
    if points:
        x_vals, y_vals = zip(*points)
        # zorder=5 ensures the points are drawn on top of the grid lines
        ax.scatter(x_vals, y_vals, color='blue', s=20, zorder=5, label='Data Points')

    # Dynamically set the camera bounds based on the root node (first rectangle)
    if rects:
        ax.set_xlim(rects[0][0], rects[0][1])
        ax.set_ylim(rects[0][2], rects[0][3])

    # Formatting
    ax.set_aspect('equal', adjustable='box')
    plt.title('QuadTree Subdivision Map')
    plt.xlabel('X Axis')
    plt.ylabel('Y Axis')
    
    # Move the legend outside the plot so it doesn't cover data
    plt.legend(loc='upper right', bbox_to_anchor=(1.25, 1))
    plt.tight_layout()
    
    # Display the window
    plt.show()

if __name__ == '__main__':
    # We will tell Python to look for a file named "qt_output.txt"
    plot_quadtree('qt_output.txt')
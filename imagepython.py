import tkinter as tk
from tkinter import messagebox, scrolledtext
import os

def build_file():
    size_str = entry_size.get()
    filename = entry_filename.get()
    
    try:
        size_mb = int(size_str)
        if size_mb <= 0:
            raise ValueError
    except ValueError:
        messagebox.showerror("Error", "Please enter a valid size in megabytes.")
        return

    try:
        with open(filename, "wb") as file:
            buffer = b'\0' * (1024 * 1024)  # 1MB buffer filled with zeros
            for i in range(size_mb):
                file.write(buffer)
                report_text = f"Written {i + 1} MB of {size_mb} MB\n"
                text_report.insert(tk.END, report_text)
                text_report.yview(tk.END)
                root.update_idletasks()
        messagebox.showinfo("Success", "File created successfully.")
    except Exception as e:
        messagebox.showerror("Error", f"Failed to create file: {str(e)}")

# Create the main window
root = tk.Tk()
root.title("DD Interface")
root.configure(bg='yellow')

# Size entry
label_size = tk.Label(root, text="Size (MB):", bg='yellow')
label_size.grid(row=0, column=0, padx=10, pady=5, sticky=tk.W)
entry_size = tk.Entry(root)
entry_size.insert(0, "100")
entry_size.grid(row=0, column=1, padx=10, pady=5)

# Filename entry
label_filename = tk.Label(root, text="Filename:", bg='yellow')
label_filename.grid(row=1, column=0, padx=10, pady=5, sticky=tk.W)
entry_filename = tk.Entry(root)
entry_filename.insert(0, "output.img")
entry_filename.grid(row=1, column=1, padx=10, pady=5)

# Report text area
label_report = tk.Label(root, text="Report:", bg='yellow')
label_report.grid(row=2, column=0, padx=10, pady=5, sticky=tk.W)
text_report = scrolledtext.ScrolledText(root, width=50, height=10, state=tk.NORMAL)
text_report.grid(row=3, column=0, columnspan=2, padx=10, pady=5)

# Build button
button_build = tk.Button(root, text="Build", command=build_file)
button_build.grid(row=0, column=2, rowspan=2, padx=10, pady=5, sticky=tk.W+tk.E+tk.N+tk.S)

# Start the Tkinter event loop
root.mainloop()


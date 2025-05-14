# Define the line
$line = "This is a line of text.`n"

# Get the size in bytes of the line
$bytesPerLine = [System.Text.Encoding]::UTF8.GetByteCount($line)

# Target size: 1GB
$targetSize = 1GB  # Equivalent to 1073741824 bytes

# Calculate how many lines are needed
$linesNeeded = [math]::Floor($targetSize / $bytesPerLine)

# File path (modify as needed)
$path = "C:\Users\Domas Se\Documents\C++ Projects\optimized_explorer\scary original.txt"

# Write the file
Remove-Item -Path $path -ErrorAction SilentlyContinue  # Remove if it exists
for ($i = 0; $i -lt $linesNeeded; $i++) {
    Add-Content -Path $path -Value $line
}

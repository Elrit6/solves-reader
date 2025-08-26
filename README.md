Simple program that reads, filters and sorts solves or averages from a csTimer export file.

The path to the input file (csTimer export) must be passed as the first parameter (Dragging it onto the program will work as well, but due to the program reading the config file from a relative path, the file must first be moved into the same directory).

Config properties:
- sessions (string array) -> Names of sessions from which solves will be read.
- solves (number) -> Size of a 'group' the solves will be read into:
    - 1 = single,
    - 2-3 = mean,
    - 4+ = average
- outputFile (string) -> Path to the file where the solves will be outputted.
- after/before (string) -> Dates than specify a time range from which the solves are read. They follow a "YYYY-MM-DD" format. For no limit, put an empty string.

Example:
```json
{
	"sessions": ["7/25", "8/25"],
	"solves": 5,
	"outputFile": "output.txt",
	"after": "2025-07-20",
	"before": "2025-08-10" 
}
```

To compile:
- c++23
- experimental stl (on g++)
- nlohmann/json library

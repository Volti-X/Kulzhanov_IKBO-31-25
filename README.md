# Kulzhanov_IKBO-31-25
Задача 1.
```
cut -d: -f1 /etc/passwd | sort
```
Задача 2.
```
grep -v "^#" /etc/protocols | awk '{print $2, $1}' | sort -nr |  head -n 5
```
Задача 3.
```
#!/bin/bash

if [ $# -ne 1 ]; then
        echo "Correct use: $0 \"banner text\"" >&2
        exit 1
fi

text="$1"
len=${#text}
line=$(printf '%*s' "$((len + 2))" '' | tr ' ' '-')

echo "+${line}+"
echo "| ${text} |"
echo "+${line}+"
```
Задача 4.
```
#!/bin/bash

if [ $# -ne 1 ]; then
        echo "Correct use: $0 <file>" >&2
        exit 1
fi

if [ ! -r "$1" ]; then
        echo "Error: cannot read file '$1'" >&2
        exit 1
fi

grep -o '[a-zA-Z_][a-zA-Z0-9_]*' "$1" | sort -u
```
Задача 5.
```
#!/bin/bash

if [ $# -ne 1 ]; then
        echo "Correct use: $0 <script>" >&2
        exit 1
fi

script="$1"

if [ ! -f "$script" ]; then
        echo "Error: '$script' is not a file" >&2
fi

chmod +x "$script"

if sudo cp "$script" /usr/local/bin; then
        echo "Registered: $script -> /usr/local/bin/$script"
else
        echo "Error: failed to copy '$script'" >&2
        exit 1
fi
```
Задача 6.
```
#!/bin/bash

if [ $# -ne 1 ]; then
        echo "Correct use: $0 <directory>" >&2
        exit 1
fi

dir="$1"

if [ ! -d "$dir" ]; then
        echo "Error: '$dir' is not a directory" >&2
        exit 1
fi

find "$dir" -type f \( -name "*.c" -o -name "*.js" -o -name "*.py" \) | while read -r file; do
        case "$file" in
                *.py)
                        pattern='^[[:space:]]*#'
                        ;;
                *.c|*.js)
                        pattern='^[[:space:]]*(//|/\*)'
                        ;;
        esac

        if head -n 1 "$file" | grep -qE "$pattern"; then
                echo "[YES] $file"
        else
                echo "[NO] $file"
        fi
done
```
Задача 7.
```
#!/bin/bash

if [ $# -ne 1 ]; then
        echo "Correct use: $0 <directory>" >&2
        exit 1
fi

dir="$1"

if [ ! -d "$dir" ]; then
        echo "Error: '$dir' is not a directory" >&2
        exit 1
fi

find "$dir" -type f -exec md5sum {} + | sort | uniq -w32 -D
```
Задача 8.
```
#!/bin/bash

if [ $# -ne 2 ]; then
        echo "Correct use: $0 <directory> <extension>" >&2
        exit 1
fi

dir="$1"
ext="$2"

if [ ! -d "$dir" ]; then
        echo "Error: '$dir' is not a directory" >&2
        exit 1
fi

archive="archive_${ext}.tar.gz"

find "$dir" -type f -name "*.${ext}" -print0 | tar -czf "$archive" --null -T -

echo "Created: $archive"
```
Задача 9.
```
#!/bin/bash

if [ $# -ne 2 ]; then
        echo "Correct use: $0 <input_file> <output_file>" >&2
        exit 1
fi

input="$1"
output="$2"

if [ ! -r "$input" ]; then
        echo "Error: cannot read '$input'" >&2
        exit 1
fi

sed -E 's/ {4}/\t/g' "$input" > "$output"

echo "Created: $output"
```
Задача 10.
```
#!/bin/bash

if [ $# -ne 1 ]; then
        echo "Correct use: $0 <directory>" >&2
        exit 1
fi

dir="$1"

if [ ! -d "$dir" ]; then
        echo "Error: '$dir' is not a directory" >&2
        exit 1
fi

find "$dir" -type f -empty
```

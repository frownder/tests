{ print $1, $2, $3 | "sort -r "}
END {
close("sort -r")
}

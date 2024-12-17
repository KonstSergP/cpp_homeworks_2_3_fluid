
height = 1000
width = 1000
mult = 10
field = []

with open("TestField.txt", "r") as file:
    out = open("BigTestField.txt", "w")

    first = file.readline()
    first = first.split()
    out.write(f"{int(first[0])*mult} {int(first[1])*mult} {first[2]} {first[3]}\n")
    out.write(file.readline())
    out.write(file.readline())

    for i in range(int(first[0])):
        ln = file.readline()
        new_string = ''.join([char * mult for char in ln]).rstrip() + "\n"
        for j in range(mult): out.write(new_string)
    out.close()
import os.path
#test
NAME_OF_CLASS = "Matrix"
PATH = os.path.dirname(os.path.realpath(__file__)) + "\\"

mainf, classf, headerf = open(PATH + "main.cpp", "r", encoding="utf-8"), \
                         open(PATH + f"{NAME_OF_CLASS}.cpp", "r", encoding="utf-8"), \
                         open(PATH + f"{NAME_OF_CLASS}.h", "r", encoding="utf-8")

finalStr = ""
includes = []
currentf = mainf
newL = currentf.readline()
for j in range(2):
    while newL.find("include") != -1:
        if newL.find(NAME_OF_CLASS) == -1:
            for i in includes:
                if i == newL:
                    break
            else:
                includes.append(newL)
        newL = currentf.readline()
    currentf = classf

finalStr += "".join(includes) + "\n"

newL = headerf.readline()
while newL.find(NAME_OF_CLASS + "(") == -1:
    finalStr += newL
    newL = headerf.readline()
headerf.close()

newL = classf.readline()
while newL.find(NAME_OF_CLASS) == -1 or newL.find("include") != -1:
    newL = classf.readline()
while newL != "":
    finalStr += "   " + newL.replace(f"{NAME_OF_CLASS}::", "")
    newL = classf.readline()
finalStr += "};\n\n"

newL = mainf.readline()
while newL != "":
    finalStr += newL
    newL = mainf.readline()

with open(PATH + "fornsu.cpp", "w", encoding="utf-8") as f:
    f.write(finalStr)

mainf.close()
classf.close()

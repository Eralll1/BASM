with open("globals/INSTSET", "r") as f:
    pairs1 = []
    pairs2 = []

    for line in f.readlines()[1:]: # skip the first line
        parts = line.split(" ")
        
        parts = [line.replace(" ", "").replace("\n", "") for line in parts]
        parts = [line for line in parts if len(line) != 0]
        if len(parts) < 2: continue
        if parts[0][0] == "#": continue
        # print(len(parts), f"'{parts}'")

        opcode = parts[0]
        name = parts[1]

        pairs1.append((opcode, name))
        pairs2.append((opcode, name))


pairs1.sort(key=lambda x: x[0])
pairs2.sort(key=lambda x: x[1])

names: dict[int, str] = {int(key, base=2):value for key,value in pairs1}

with open("globals/operations.c", "w") as f:
    f.write("// *--------------------------------------------------*\n")
    f.write("// |   Auto generated. To edit use gen_op_codes.py    |\n")
    f.write("// *--------------------------------------------------*\n\n\n")

    f.write('#include "op_helper.h"\n')
    f.write('#include "functions.h"\n\n')


    f.write(f'size_t const OP_CODE_COUNT = {len(pairs1)};\n')

    f.write("opcode_map_obj const OP_NAME_MAP[] = {\n")
    for code, name in pairs2:
        f.write(f'    {{{str(int(code, base=2))}, "{name}"}},\n')
    f.write("};\n\n")

    f.write(f'func_ptr_t const OP_CODE_FUNC_MAP[] = {{ \n')
    for i in range(256):
        if i in names.keys():
            f.write(f'    &__spu__{names[i].lower()},\n')
        else:
            f.write(f'    NULL,\n')
    f.write(f'}};\n')


    # f.write("FullOP const _OP_SORTED_CODES[] = {\n")
    # for code, name, argc in pairs1:
    #     f.write(f'    {{{str(int(code, base=2))}, "{name}", {argc}}},\n')
    # f.write("};\n\n")

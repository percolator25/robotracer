def src = new File('build/index.html').text

def sb = new StringBuilder()

def data = src.toList().collate(80)*.join().collect {
    return "\"" +
            it
                    .replace('\\', "\\\\")
                    .replace('\"', "\\\"") +
            "\""
}

sb.append('#include <Arduino.h>\n')
sb.append('class Data\n')
        .append('{\n')
        .append('public:\n')
        .append('    static const char *index;\n')
        .append('};\n\n')
        .append('const char *Data::index = ')

data.eachWithIndex { String entry, int i ->
    if (i == 0) {
        sb.append(entry).append("\n")
    } else {
        sb.append("                    ").append(entry).append("\n")
    }
}

sb.append(";")


new File("src/Data.h").write(sb.toString())
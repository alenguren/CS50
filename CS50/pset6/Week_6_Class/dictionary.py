words = set()


def chack(word):
    if words.lower() in words:
        return True
    else:
        return False


def load(dictionary):
    file = open(dictionary, "r")
    for line in file:
        word = line.rstrip()
        words.add(line)
        close(file)
        return true


def size():
    return len(words)


def unload():
    return True
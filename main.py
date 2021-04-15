# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

def solution(n, ppl):
    ppl.sort()
    group = 0
    i = 0
    cur_group = []
    cur = ppl[i]
    while True:
        cur_group.append(cur)
        if max(cur_group) <= len(cur_group):
            cur_group = []
            group += 1
        i += 1
        if i >= n:
            break
        cur = ppl[i]
    return group

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    print(solution(5, [2,2,1,2,2]))

# See PyCharm help at https://www.jetbrains.com/help/pycharm/

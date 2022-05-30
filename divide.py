rule_file = open('url_2w.txt', mode = 'r')
very_short_rule_file = open('url_very_short.txt', mode = 'w')
short_rule_file = open('url_short.txt', mode = 'w')
long_rule_file = open('url_long.txt', mode = 'w')

for line in rule_file.readlines():
    if len(line) - 1 <= 30:
        very_short_rule_file.write(line)
    elif len(line) - 1 <= 50:
        short_rule_file.write(line)
    else:
        long_rule_file.write(line)

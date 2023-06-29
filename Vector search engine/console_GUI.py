import scrapper
import lemmatization
import matrix
import search
import article_hook

PAGES_AMOUNT = 100
INPUT_FOLDER = 'Articles'
OUTPUT_FOLDER = 'PreprocessedArticles'
JSON_FOLDER = 'Stored'
IS_SEQ = False


def process_search():
    while True:
        print("Enter query (Format: word(w), word(w), ...) or type exit")
        str_query = input()
        if str_query == 'exit':
            break
        str_query = str_query.split(',')
        parsed_query = dict()
        for term in str_query:
            word = term[:term.find('(')]
            w = term[term.find('(') + 1:term.find(')')]
            parsed_query[str(word)] = float(w)
        print("---------------Search---------------")
        if IS_SEQ:
            result = search.sequential_search(parsed_query)
        else:
            result = search.process_query(parsed_query)
        print("---------------Result---------------")
        for i, f in enumerate(result):
            if i > 10:
                break
            # print("Document id: ", f[0], " ,similarity: ", f[1])
            print(article_hook.get_header_and_href(INPUT_FOLDER, int(f[0])), "similarity: ", f[1])
        print("------------------------------------")


def search_engine_configuration():
    global JSON_FOLDER
    global OUTPUT_FOLDER
    global PAGES_AMOUNT
    global IS_SEQ
    while True:
        print("Choose:")
        print("1. Load configuration from json")
        print("2. Generate new")
        print("3. Generate new sequential version")
        print("4. Sequential version from json")
        choice = int(input())
        if choice not in range(1, 5):
            continue
        break

    if choice == 1:
        matrix.load_matrix(JSON_FOLDER)
        IS_SEQ = False
        print("Configuration loaded")
    elif choice == 2:
        matrix.generate_sparse_matrix(OUTPUT_FOLDER, PAGES_AMOUNT)
        matrix.process_vectors()
        matrix.save_matrix(JSON_FOLDER)
        IS_SEQ = False
        print("Configuration generated and saved")
    elif choice == 3:
        matrix.generate_seq_matrix(OUTPUT_FOLDER, PAGES_AMOUNT)
        matrix.save_matrix(JSON_FOLDER, True)
        IS_SEQ = True
        print("Configuration generated and saved")
    elif choice == 4:
        matrix.load_matrix(JSON_FOLDER, True)
        IS_SEQ = True
        print("Configuration loaded")


def preprocessing():
    global INPUT_FOLDER
    global OUTPUT_FOLDER
    global PAGES_AMOUNT

    while True:
        print("Do you want to choose destination folder path? y/n")
        choice = input()
        if choice not in ['y', 'n']:
            continue
        break

    if choice == 'y':
        print("Enter the destination folder path")
        OUTPUT_FOLDER = input()

    lemmatization.process_lemmatization(INPUT_FOLDER, OUTPUT_FOLDER, PAGES_AMOUNT, 7, 'english')


def scrapping():
    global PAGES_AMOUNT
    global INPUT_FOLDER

    while True:
        print("From which page do you want to start? (1..500)")
        start = int(input())
        print("start ", start)
        if start not in range(1, 501):
            print("Incorrect page")
            continue
        break
    while True:
        print("At which page do you want to end scrapping? (1..700)")
        end = int(input())
        print("end ", end)
        if end not in range(1, 701):
            print("Incorrect page")
            continue
        break

    while True:
        print("Do you want to choose destination folder path? y/n")
        choice = input()
        if choice not in ['y', 'n']:
            continue
        break

    if choice == 'y':
        print("Enter the destination folder path")
        INPUT_FOLDER = input()

    PAGES_AMOUNT = end - start + 1

    scrapper.scrape(start, end, INPUT_FOLDER)


def welcome_menu():

    print("-------Welcome------")
    print("Choose option:")
    print("1. Scrapper")
    print("2. Launch preprocessing")
    print("3. Search engine configuration")
    print("4. Search")
    print("5. Exit\n")
    choice = int(input())
    if choice not in range(1, 6):
        print("Choose one of the suggested.(1-5)")

    return choice



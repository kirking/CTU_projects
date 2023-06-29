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


def process_search(str_query):
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
    result_list = list()
    for i, f in enumerate(result):
        if i > 10:
            break
            # print("Document id: ", f[0], " ,similarity: ", f[1])
        hooked_article = article_hook.get_header_and_href(INPUT_FOLDER, int(f[0]))
        result_list.append({"name": hooked_article[0], "similarity": f[1], "link": hooked_article[1]})
        # print(article_hook.get_header_and_href(INPUT_FOLDER, int(f[0])), "similarity: ", f[1])
        # print("------------------------------------")
    return result_list


def search_engine_configuration(choice):
    global JSON_FOLDER
    global OUTPUT_FOLDER
    global PAGES_AMOUNT
    global IS_SEQ
    print("BACKEND ENGINE CONFIG")
    if choice == 1:
        matrix.generate_sparse_matrix(OUTPUT_FOLDER, PAGES_AMOUNT)
        matrix.process_vectors()
        matrix.save_matrix(JSON_FOLDER)
        IS_SEQ = False
        print("Configuration generated and saved")

    elif choice == 2:
        matrix.load_matrix(JSON_FOLDER)
        IS_SEQ = False
        print("Configuration loaded")
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

    lemmatization.process_lemmatization(INPUT_FOLDER, OUTPUT_FOLDER, PAGES_AMOUNT, 7, 'english')


def scrapping(start, end):
    global PAGES_AMOUNT
    global INPUT_FOLDER

    PAGES_AMOUNT = end - start + 1

    scrapper.scrape(start, end, INPUT_FOLDER)




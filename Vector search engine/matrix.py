import collections
import math
import json
articles_count = 0
sparse_matrix = dict()
processed_vectors = dict()


def generate_sparse_matrix(input_folder, doc_amount):
    global sparse_matrix
    sparse_matrix = dict()
    # generating tf matrix
    global articles_count
    articles_count = doc_amount*7
    for article_num in range(1, articles_count+1):
        with open(input_folder + '/' + str(article_num) + ".txt", "r", encoding="utf-8") as lemmatized_article:
            words = collections.Counter(list(lemmatized_article.read().split(' ')))
            max_occur = max(words.values())
            for word in words:
                if word not in sparse_matrix.keys():
                    sparse_matrix[word] = dict()
                sparse_matrix[word][str(article_num)] = round(words[word]/max_occur, 2)
        lemmatized_article.close()

# generating sparse matrix
    for word in sparse_matrix:
        idf = math.log(articles_count/len(sparse_matrix[word].keys()))

        for doc_id in sparse_matrix[word]:
            sparse_matrix[word][doc_id] = round(idf*sparse_matrix[word][doc_id], 2)

# Normalization
    maximum = 0
    for word in sparse_matrix.keys():
        tmp_max = max(sparse_matrix[word].values())
        if maximum < tmp_max:
            maximum = tmp_max

    for word in sparse_matrix:
        for doc_id in sparse_matrix[word]:
            sparse_matrix[word][doc_id] = round(sparse_matrix[word][doc_id]/maximum, 2)


def process_vectors():
    global processed_vectors
    processed_vectors = dict()
    for doc_id in range(1, articles_count + 1):
        processed_vectors[str(doc_id)] = 0
        for word in sparse_matrix.keys():
            if sparse_matrix[word].get(str(doc_id)):
                processed_vectors[str(doc_id)] += sparse_matrix[word][str(doc_id)] ** 2


def generate_seq_matrix(input_folder, doc_amount):
    global sparse_matrix
    sparse_matrix = dict()
    global articles_count
    articles_count = doc_amount * 7
    all_words = dict()
    for article_num in range(1, articles_count + 1):
        with open(input_folder + '/' + str(article_num) + ".txt", "r", encoding="utf-8") as lemmatized_article:
            words = collections.Counter(list(lemmatized_article.read().split(' ')))
            sparse_matrix[str(article_num)] = dict()
            max_occur = max(words.values())
            for word in words.keys():
                if word not in all_words.keys():
                    all_words[word] = 1
                else:
                    all_words[word] += 1

                sparse_matrix[str(article_num)][word] = round(words[word] / max_occur, 2)
        lemmatized_article.close()
    # idf calculation
    for word in all_words.keys():
        all_words[word] = math.log(articles_count/all_words[word])
    # weights calculation
    for doc in sparse_matrix.keys():
        for word in sparse_matrix[doc].keys():
            sparse_matrix[doc][word] = round(all_words[word]*sparse_matrix[doc][word], 2)

    maximum = 0
    for doc_id in sparse_matrix.keys():
        tmp_max = max(sparse_matrix[doc_id].values())
        if maximum < tmp_max:
            maximum = tmp_max

    for doc_id in sparse_matrix:
        for word in sparse_matrix[doc_id]:
            sparse_matrix[doc_id][word] = round(sparse_matrix[doc_id][word] / maximum, 2)


def save_matrix(output_folder, seq=False):
    if seq:
        matrix_name = "/sequence_matrix.json"
    else:
        matrix_name = "/sparse_matrix.json"
    with open(output_folder + matrix_name, "w", encoding="utf-8") as output:
        json.dump(sparse_matrix, output)
        output.close()
    with open(output_folder + "/processed_vectors.json", "w", encoding="utf-8") as output:
        json.dump(processed_vectors, output)
    with open(output_folder + "/stored_articles.json", "w", encoding="utf-8") as output:
        json.dump({"Articles amount": articles_count}, output)
        output.close()


def load_matrix(input_path, seq=False):
    global articles_count
    global sparse_matrix
    global processed_vectors
    with open(input_path + "/stored_articles.json", "r", encoding="utf-8") as input_articles:
        articles_count = json.load(input_articles)["Articles amount"]
        input_articles.close()
    if seq:
        matrix_name = "/sequence_matrix.json"
    else:
        matrix_name = "/sparse_matrix.json"

    with open(input_path + matrix_name, "r", encoding="utf-8") as input_matrix:

        sparse_matrix = json.load(input_matrix)
        input_matrix.close()

    if not seq:
        with open(input_path + "/processed_vectors.json", "r", encoding="utf-8") as vectors:
            processed_vectors = json.load(vectors)
            input_matrix.close()

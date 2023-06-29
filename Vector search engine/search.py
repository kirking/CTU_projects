import matrix
import math

threshold = 0.5

sparse_matrix = matrix.sparse_matrix
doc_amount = matrix.articles_count
vectors_processed_weights = matrix.processed_vectors


def cos_similarity(query, document, processed_weight=0):
    w_sum = 0
    norm1 = 0
    norm2 = 0
    process_here = True
    if processed_weight != 0:
        process_here = False
        norm2 = processed_weight
    for word in query:
        if document.get(word):
            doc_weight = document[word]
        else:
            doc_weight = 0
        w_sum += query[word] * doc_weight
        # print(query[word], " * ", doc_weight, '+=', w_sum)
        norm1 += query[word] ** 2

    if process_here:
        for word in document:
            norm2 += document[word] ** 2
            # print("sum = ", w_sum, "norm1 = ", norm1, "norm2 = ", norm2)

    if w_sum == 0:
        return 0
    # print("--------------")

    result = w_sum / math.sqrt(norm1 * norm2)

    return result


def process_query(input_query):
    global threshold
    global sparse_matrix
    global vectors_processed_weights
    global doc_amount
    result_list = []
    sparse_matrix = matrix.sparse_matrix
    doc_amount = matrix.articles_count
    vectors_processed_weights = matrix.processed_vectors
    found_docs = dict()
    for q_word in input_query.keys():
        tmp = sparse_matrix.get(q_word) # id s vahami
        # print("------before-------", found_docs)
        if tmp:
            for id_weight in tmp.keys():
                # print(id_weight)
                if not found_docs.get(str(id_weight)):
                    found_docs[id_weight] = dict()
                found_docs[id_weight][str(q_word)] = tmp[id_weight]
    for doc in found_docs.keys():
        # print("Similarity id", doc)
        tmp_sim = round(cos_similarity(input_query, found_docs[doc], vectors_processed_weights[doc]),2)
        if tmp_sim != 0:
        # if tmp_sim >= threshold:
            result_list.append((doc, tmp_sim))

    result_list = sorted(result_list, key=lambda tup: tup[1], reverse=True)
    # print(result_list)
    return result_list


def sequential_search(input_query):
    global threshold
    global sparse_matrix
    global doc_amount
    sparse_matrix = matrix.sparse_matrix
    doc_amount = matrix.articles_count

    result_list = list()

    for doc_id in range(1, doc_amount + 1):

        similarity = round(cos_similarity(sparse_matrix[str(doc_id)], input_query), 2)

        # if similarity > threshold:
        if similarity != 0:
            result_list.append((doc_id, similarity))

    result_list = sorted(result_list, key=lambda tup: tup[1], reverse=True)
    # print(result_list)
    return result_list



# sequential_search([('spring', 1)])
# process_query([('fact', 0.9), ('silent', 0.9), ('spring', 0.8)])



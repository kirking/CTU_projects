def hook_article(article_id):
    if article_id // 7 == 0:
        page = 1
    else:
        page = article_id//7 + 1

    if article_id % 7 == 0:
        article = 7
        page -= 1
    else:
        article = article_id % 7

    article_name = str(page) + "_" + str(article) + '.txt'
    return article_name


def get_header_and_href(input_folder, article_id):

    with open(input_folder + "/" + hook_article(article_id), "r", encoding="utf-8") as file:
        lines = file.readlines()
        header = lines[0].encode("ISO-8859-1").decode('utf-8')
        href = lines[-1].encode("ISO-8859-1").decode('utf-8')
        # print(hook_article(article_id))
    return header, href


import re
import nltk
from nltk.corpus import stopwords
from nltk.corpus import wordnet
from nltk.stem import WordNetLemmatizer


# Lemmatize with POS Tag
def get_wordnet_pos(word):
    """Map POS tag to first character lemmatize() accepts"""
    tag = nltk.pos_tag([word])[0][1][0].upper()
    tag_dict = {"J": wordnet.ADJ,
                "N": wordnet.NOUN,
                "V": wordnet.VERB,
                "R": wordnet.ADV}
    return tag_dict.get(tag, wordnet.NOUN)


def lemmatize_file(in_file_path, out_file_path, stopwords_set):
    # read file
    raw_article = open(in_file_path, "r", encoding="utf-8")

    # replace new line symbols, delete reference
    data = raw_article.read().lower().replace('\n', ' ')
    data = ' '.join(data.split(" ")[:-1])

    raw_article.close()

    # delete unnecessary symbols
    data = re.sub(r'[^a-z ]', ' ', data)
    words = nltk.word_tokenize(data)
    prepared_article = list()
    for r in words:
        if r not in stopwords_set:
            prepared_article.append(r)

    # Init Lemmatizer
    lemmatizer = WordNetLemmatizer()
    lemmatized_output = ' '.join([lemmatizer.lemmatize(w, get_wordnet_pos(w)) for w in prepared_article])

    # write file
    with open(out_file_path, 'w', encoding="utf-8") as txt_file:
        txt_file.write(lemmatized_output)
    txt_file.close()
    return


# language = english
def process_lemmatization(input_folder, output_folder, pages, article_per_page, language):
    stop_words = set(nltk.corpus.stopwords.words(language))

    counter = 1
    for page in range(1, pages+1):
        for article in range(1, article_per_page+1):
            lemmatize_file(input_folder + '/' + str(page)+'_'+str(article)+'.txt',
                           output_folder + '/' + str(counter)+'.txt',
                           stop_words)
            counter += 1

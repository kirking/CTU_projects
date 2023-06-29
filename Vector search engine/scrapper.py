from bs4 import BeautifulSoup
import requests


def scrape(first_page, pages_amount, dest_folder):
    headers = {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36'}
    url = 'https://www.mentalfloss.com/section/science?page='
    # path = 'Articles/'
    # pages = 100

    for page_num in range(first_page, pages_amount + 1):
        article_cnt = 1

        html_text = requests.get(url + str(page_num), headers)

        soup = BeautifulSoup(html_text.text.encode("ISO-8859-1").decode('utf-8'), 'lxml')

        articles = (i.find('a').get('href') for i in soup.find_all('article'))

        for article_url in articles:
            header = BeautifulSoup(requests.get(article_url).text.encode("ISO-8859-1").decode('utf-8'), 'lxml').find('article').find("h1")

            with open(dest_folder + '/' + str(page_num) + "_" + str(article_cnt) + '.txt', 'w', encoding="utf-8") as f:
                f.write(header.text.replace('\xa0', ' ') + "\n\n")
                tmp = BeautifulSoup(requests.get(article_url).text.encode("ISO-8859-1").decode('utf-8'), 'lxml').find('article').find('main').find_all('p')
                tmp = tmp[:-1]
                for i in tmp:
                    txt = i.text.replace('\xa0', ' ')
                    f.write(txt)
                    f.write("\n")
                f.write("\n"+article_url)
                article_cnt += 1
                f.close()

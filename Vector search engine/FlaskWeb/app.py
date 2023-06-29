from flask import Flask, render_template, request, redirect
import backend
import time
app = Flask(__name__)

no = []


def get_results(input_query):
    return backend.process_search(input_query)


def scrapper(start, end):
    backend.scrapping(start, end)


def preprocess():
    backend.preprocessing()


def engine_settings(option):
    backend.search_engine_configuration(option)


@app.route("/", methods=['POST', 'GET'])
def index():
    if request.method == 'POST':
        input_query = request.form['title']
        if input_query == "" or input_query == " ":
            return redirect('/')
        else:

            input_query = input_query.replace(' ', '')

            try:
                start_time = time.time()
                articles = get_results(input_query)
                print(time.time() - start_time)
            except:

                return render_template("index.html", articles=no)

            return render_template("results.html", input=input_query, articles=articles)
    return render_template("index.html", articles=no)


@app.route("/results")
def results(req, articles):
    return render_template("results.html",
                           articles=articles,
                           input=req)


@app.route("/Scrapper", methods=['POST', 'GET'])
def Scrapper():
    if request.method == 'POST':
        start = int(request.form['start'])
        end = int(request.form['end'])
        print('Start = ', start, "End = ", end)
        scrapper(start, end)
        preprocess()
        return redirect('/Scrapper')
    return render_template("Scrapper.html")


@app.route("/Engine", methods=['POST', 'GET'])
def Engine():
    if request.method == 'POST':
        select = int(request.form["engine_select"])
        print(select)
        engine_settings(select)
        return redirect('/Engine')

    return render_template("Engine.html")


if __name__ == '__main__':
    app.run(debug=True)

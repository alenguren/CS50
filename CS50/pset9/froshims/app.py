from flask import Flask, render_template, request

app = Flask(__name__)

REGISTRANTS = {}

SPORTS = [
    "Basquetball",
    "Soccer",
    "Ultimate Frisbee"
]

@app.route("/")
def index():
    return render_template("index.html" , sports=SPORTS)

@app.route("/register", methods=["GET", "POST"])
def register():
    name = request.form.get("name")
    if not name:
        return render_template("failure.html")
    sport = request.form.get("sport")
    if sport not in SPORTS:
        return render_template("failure.html")
    REGISTRANTS[name] = sport
    return render_template("success.html")

@app.route("/registrants", methods=["GET", "POST"])
def registrants():
    return render_template("registrants.html", registrants=REGISTRANTS)
import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query the database for the user's id, cash balance, and stock holdings
    user_id = session["user_id"]
    user = db.execute("SELECT username FROM users WHERE id = :id", id=user_id)[0]
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=user_id)[0]["cash"]
    stocks = db.execute("SELECT symbol, name, price, SUM(shares) AS shares_owned FROM transactions WHERE user_id = :user_id "
        "GROUP BY symbol "
        "HAVING shares_owned > 0", user_id=user_id)

    # Calculate the total value of each holding and the overall portfolio value
    total_value = 0
    for stock in stocks:
        stock_value = stock["price"] * stock["shares_owned"]
        stock["value"] = usd(stock_value)
        total_value += stock_value

    # Format the cash balance and total value as USD strings
    cash_balance_str = usd(cash)
    total_value_str = usd(total_value + cash)

    return render_template("index.html", stocks=stocks, cash_balance=cash_balance_str, total_value=total_value_str, username=user["username"])


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Ensure symbol was submitted
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide symbol")

        # Ensure shares was submitted and is a positive integer
        try:
            shares = int(request.form.get("shares"))
            if shares <= 0:
                raise ValueError
        except ValueError:
            return apology("shares must be a positive integer")

        # Look up current price of the stock
        quote = lookup(symbol)
        if not quote:
            return apology("invalid symbol")

        # Calculate the cost of the shares
        item_name = quote["name"]
        price = quote["price"]
        cost = shares * price

        # Get the user's session
        user_id = session["user_id"]

        # Get the user's current cash balance
        user_cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]

        # Ensure the user has enough cash to make the purchase
        if cost > user_cash:
            return apology("Not enough cash!")

        # Record the purchase in the transactions table
        db.execute("INSERT INTO transactions (user_id, name, symbol, shares, price, type) VALUES (:user_id, :name, :symbol, :shares, :price, :type)",
                   user_id=session["user_id"], name=item_name, symbol=symbol.upper(), shares=shares, price=price, type='BUY')

        # Deduct the cost from the user's cash balance
        db.execute("UPDATE users SET cash = cash - :cost WHERE id = :user_id", cost=cost, user_id=session["user_id"])

        flash(f"Bought {shares} shares of {quote['name']} ({symbol}) for ${cost:.2f}.")
        return redirect("/")

    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Retrieve all transactions for the current user
    transactions = db.execute("SELECT type, symbol, price, shares * price AS total, shares, timestamp FROM transactions WHERE user_id = :user_id ORDER BY timestamp ASC", user_id=session["user_id"])

    # Render transactions history page
    return render_template("history.html", transactions=transactions)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password")

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide symbol")

        quote = lookup(symbol)
        if not quote:
            return apology("invalid symbol")

        # Get the number of shares owned by the user for the given symbol
        shares = db.execute(
            "SELECT COALESCE(SUM(shares), 0) AS shares_owned FROM transactions WHERE user_id = :user_id AND symbol = :symbol",
            user_id=session["user_id"],
            symbol=symbol.upper()
        )
        shares_owned = shares[0]["shares_owned"]

        return render_template("quoted.html", name=quote["name"], symbol=quote["symbol"], price=usd(quote["price"]), shares_owned=shares_owned)

    return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation")

        # Ensure password and confirmation match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username doesn't already exist
        if len(rows) > 0:
            return apology("username already exists")

        # Insert the new user into the users table
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                   request.form.get("username"), generate_password_hash(request.form.get("password")))

        # Redirect user to home page
        flash("Registered!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        # Retrieve list of stocks owned by the user
        user_stocks = db.execute("SELECT symbol FROM transactions WHERE user_id = :user_id GROUP BY symbol", user_id=session["user_id"])
        return render_template("sell.html", stocks=user_stocks)
    else:
        # Process the sale of shares
        stock_symbol = request.form.get("symbol")
        shares_to_sell = request.form.get("shares")

        # Validate user input
        if not stock_symbol:
            return apology("Please select a stock")
        if not shares_to_sell or not shares_to_sell.isnumeric() or int(shares_to_sell) <= 0:
            return apology("Please enter a valid number of shares to sell")

        # Check if user has enough shares to sell
        user_shares = db.execute("SELECT SUM(shares) AS total_shares FROM transactions WHERE user_id = :user_id AND symbol = :symbol GROUP BY symbol", user_id=session["user_id"], symbol=stock_symbol)
        if not user_shares or user_shares[0]["total_shares"] < int(shares_to_sell):
            return apology("You don't own enough shares to sell")

        # Retrieve stock information and calculate sale price
        stock_info = lookup(stock_symbol)
        total_sale_price = int(shares_to_sell) * stock_info["price"]

        # Update transaction and user data in database
        db.execute("INSERT INTO transactions (user_id, name, symbol, shares, price, type, timestamp) VALUES (:user_id, :name, :symbol, :shares, :price, :type, strftime('%Y-%m-%d %H:%M:%S','now','localtime'))", user_id=session["user_id"], name=stock_info["name"], symbol=stock_symbol, shares=-int(shares_to_sell), price=stock_info["price"], type="SELL")
        db.execute("UPDATE users SET cash = cash + :total_sale_price WHERE id=:user_id", total_sale_price=total_sale_price, user_id=session["user_id"])

        # Display success message with stock name and redirect to homepage
        flash(f"Sold {shares_to_sell} shares of {stock_symbol} for ${total_sale_price:.2f}!")
        return redirect("/")




@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Allow user to change password"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure current password was submitted
        if not request.form.get("current_password"):
            return apology("must provide current password")

        # Ensure new password was submitted
        elif not request.form.get("new_password"):
            return apology("must provide new password")

        # Ensure confirmation password was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation password")

        # Ensure new password and confirmation password match
        elif request.form.get("new_password") != request.form.get("confirmation"):
            return apology("new password and confirmation password must match")

        # Query database for user
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])

        # Ensure current password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("current_password")):
            return apology("invalid current password")

        # Update password hash in database
        db.execute("UPDATE users SET hash = :hash WHERE id = :id",
                   hash=generate_password_hash(request.form.get("new_password")), id=session["user_id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("change_password.html")


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    if request.method == "POST":
        # Get the amount entered by the user
        amount = request.form.get("amount")
        # Check if the entered amount is valid
        if not amount or not amount.isdigit() or float(amount) <= 0:
            return apology("Invalid amount")
        # Add the amount to the user's cash balance in the database
        db.execute("UPDATE users SET cash = cash + :amount WHERE id = :user_id",
                   amount=float(amount), user_id=session["user_id"])
        # Flash a success message and redirect to the home page
        flash("Cash added successfully")
        return redirect("/")
    else:
        # If the request method is GET, render the add_cash.html template
        return render_template("add_cash.html")

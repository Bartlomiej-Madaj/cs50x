import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
import re

from helpers import apology, login_required, lookup, usd, check_valid_password

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
    user_id = session.get("user_id")
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    stocks = db.execute("SELECT name, symbol, buy_price, shares, date FROM stocks JOIN transactions ON stocks.id = transactions.stock_id JOIN users ON users.id = transactions.user_id WHERE transactions.user_id = ? ORDER BY date DESC", user_id)
    user_stocks = []
    for stock in stocks:
        user_stock = {}
        if stock['symbol'] not in [user_stock['symbol'] for user_stock in user_stocks]:
            user_stock['symbol'] = stock['symbol']
            user_stock['name'] = stock['name']
            user_stock['shares'] = stock['shares']
            user_stock['current_price'] = round(lookup(stock['symbol']).get('price', 0), 2)
            user_stocks.append(user_stock)
        else:
            for exist_user_stock in user_stocks:
                if exist_user_stock['symbol'] == stock['symbol']:
                    exist_user_stock['shares'] += stock['shares']

    user_stocks = [item for item in user_stocks if item['shares'] > 0]
    stocks_price = 0
    for stock in user_stocks:
        stocks_price += float(stock['current_price']) * stock['shares']
    total = float(user_cash[0]['cash'] + stocks_price)
    return render_template("index.html", stocks=user_stocks, total=round(total, 2), cash=round(float(user_cash[0]['cash']), 2))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        user_id = session.get("user_id")
        entered_symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        stock = lookup(entered_symbol.strip())
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        curr_stock_symbols = db.execute(
            "SELECT symbol FROM stocks JOIN transactions ON stocks.id = transactions.stock_id JOIN users ON users.id = transactions.user_id WHERE transactions.user_id = ?", user_id)
        if stock is None:
            return apology("This symbol does not exist!")
        elif len(entered_symbol) == 0:
            return apology("Symbol is required!")
        elif int(shares) < 1:
            return apology("Number of shares must be a positive number!")
        elif user_cash[0]['cash'] < int(shares) * float(stock['price']):
            return apology("You do not have enough money to buy this number of shares!")
        else:
            stock_symbols = db.execute("SELECT symbol FROM stocks")
            users_id = db.execute("SELECT user_id FROM transactions")
            if entered_symbol not in [stock_symbol['symbol'] for stock_symbol in stock_symbols]:
                db.execute("INSERT INTO stocks (name, symbol) VALUES (?, ?)",
                           stock["name"], stock["symbol"])
            stock_id = db.execute(
                "SELECT id FROM stocks WHERE symbol = ?", entered_symbol)
            if user_id not in [user_id['user_id'] for user_id in users_id]:
                db.execute("INSERT INTO transactions (user_id, stock_id, shares, buy_price, date) VALUES (?, ?, ?, ?, ?)",
                           user_id, stock_id[0]['id'], shares, stock["price"], datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
            elif entered_symbol not in [curr_stock_symbol['symbol'] for curr_stock_symbol in curr_stock_symbols]:
                db.execute("INSERT INTO transactions (user_id, stock_id, shares, buy_price, date) VALUES (?, ?, ?, ?, ?)",
                           user_id, stock_id[0]['id'], shares, stock["price"], datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
            else:
                db.execute("INSERT INTO transactions (user_id, stock_id, shares, buy_price, date) VALUES (?, ?, ?, ?, ?) ", user_id, stock_id[0]['id'],  int(shares), stock["price"], datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
            current_cash = db.execute(
                "SELECT cash FROM users WHERE id = ?", user_id)
            rest_cash = float(
                current_cash[0]['cash']) - int(shares) * float(stock['price'])
            db.execute("UPDATE users SET cash = ? WHERE id = ?",
                       rest_cash, user_id)
            return redirect("/")
    else:
        return render_template("quoted.html", stock=None)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session.get("user_id")
    transactions = db.execute("SELECT name, symbol, buy_price, sale_price, date, shares FROM transactions JOIN users ON transactions.user_id = users.id JOIN stocks ON transactions.stock_id = stocks.id WHERE users.id = ? ", user_id)

    bought_stocks = []
    sold_stocks = []
    for transaction in transactions:
        if transaction['buy_price'] is None:
            transaction['shares'] = -int(transaction['shares'])
            sold_stocks.append(transaction)
        else:
            bought_stocks.append(transaction)
    return render_template("history.html", bought_stocks=bought_stocks, sold_stocks=sold_stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

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
        entered_symbol = request.form.get("symbol")
        if lookup(entered_symbol) is None:
            return apology("This symbol does not exist!")
        elif len(entered_symbol) == 0:
            return apology("Symbol is required!")
        stock = lookup(entered_symbol)
        return render_template("quoted.html", stock=stock)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        entered_username = request.form.get("username")
        password = request.form.get("password")

        confirmation = request.form.get("confirmation")
        usernames = db.execute("SELECT username FROM users;")
        for username in usernames:
            if username == entered_username:
                return apology("This username already exists!")
        if len(entered_username) == 0:
            return apology("Username is required!")
        elif not check_valid_password(password):
            return apology("The password should be at least 8 characters long and contain numbers, uppercase and lowercase letters, as well as special characters!", 403)
        elif password != confirmation:
            return apology("Passwords do not match!")
        hashed_password = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?);",
                   entered_username, hashed_password)
        return render_template("login.html")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session.get("user_id")

    if request.method == "POST":
        entered_shares = request.form.get("shares")
        entered_symbol = request.form.get("symbol")
        current_stock = lookup(entered_symbol)
        if current_stock is None:
            return apology("This symbol does not exist!")

        stocks = db.execute("SELECT name, symbol, buy_price, shares FROM stocks JOIN transactions ON stocks.id = transactions.stock_id JOIN users ON users.id = transactions.user_id WHERE transactions.user_id = ? AND stocks.symbol = ? ORDER BY transactions.date DESC", user_id, entered_symbol)
        user_stock = {}
        for stock in stocks:
            if  'symbol' not in user_stock:
                user_stock['symbol'] = stock['symbol']
                user_stock['name'] = stock['name']
                user_stock['shares'] = stock['shares']
            else:
                user_stock['shares'] += stock['shares']

        if int(entered_shares) < 0 and int(entered_shares) > int(user_stock['shares']):
            return apology("You do not have this number of shares!")
        else:
            stock_id = db.execute(
                "SELECT id FROM stocks WHERE symbol = ?", entered_symbol)
            db.execute("INSERT INTO transactions (shares, sale_price, date, stock_id, user_id) VALUES (?, ?, ?, ?, ?)", -int(entered_shares), float(current_stock['price']), datetime.now().strftime('%Y-%m-%d %H:%M:%S'), stock_id[0]['id'], user_id)

            current_cash = db.execute("SELECT cash FROM users WHERE id =?", user_id)
            cash = float(current_cash[0]['cash']) + int(entered_shares) * float(current_stock['price'])
            db.execute("UPDATE users SET cash = ?", cash)
            return redirect("/")
    else:
        stocks = db.execute(
    "SELECT symbol, shares, name FROM stocks JOIN transactions ON stocks.id = transactions.stock_id JOIN users ON users.id = transactions.user_id WHERE transactions.user_id = ?", user_id)

        current_stocks = {}
        for stock in stocks:
            current_stocks[stock['symbol']] = current_stocks.get(stock['symbol'], 0) + int(stock['shares'])

        current_stocks = {key: value for key, value in current_stocks.items() if value != 0}
        return render_template("sell.html", stocks=current_stocks, tests= stocks)


@app.route("/show", methods=["GET", "POST"])
@login_required
def show():
    """Show shares of stock to sell"""
    if request.method == "POST":
        user_id = session.get("user_id")
        entered_symbol = request.form.get("symbol")
        current_stock = lookup(entered_symbol)
        stocks = db.execute("SELECT name, symbol, buy_price, shares, transactions.id FROM stocks JOIN transactions ON stocks.id = transactions.stock_id JOIN users ON users.id = transactions.user_id WHERE transactions.user_id = ? AND stocks.symbol = ? ORDER BY date DESC ", user_id, entered_symbol)
        user_stock = {}
        for stock in stocks:
            if  'symbol' not in user_stock:
                user_stock['symbol'] = stock['symbol']
                user_stock['name'] = stock['name']
                user_stock['shares'] = stock['shares']
            else:
                user_stock['shares'] += stock['shares']
        return render_template("show_stock.html", stock=user_stock, shares=user_stock['shares'], current_price=round(current_stock['price'], 2))
    else:
        return redirect("/")

@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """Add some cash to wallet"""
    if request.method == "POST":
        user_id = session.get("user_id")
        entered_cash = request.form.get("cash")
        if float(entered_cash) < 0:
            return apology("You can't add money you don't have! Add a greater than zero amount of money!")
        current_cash = db.execute("SELECT cash FROM users WHERE id =?", user_id)
        cash = float(current_cash[0]['cash']) + float(entered_cash)
        db.execute("UPDATE users SET cash = ?", cash)
        return redirect("/")
    else:
        return redirect("/")

@app.route("/change-password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change user password"""
    user_id = session.get("user_id")
    if request.method == "POST":
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")
        # Query database for username
        rows = db.execute("SELECT hash FROM users WHERE id = ?", user_id)
        if not check_password_hash(rows[0]["hash"], current_password):
            return apology("invalid current password", 403)
        # Ensure password was submitted
        elif not check_valid_password(new_password):
            return apology("The password should be at least 8 characters long and contain numbers, uppercase and lowercase letters, as well as special characters!", 403)

        hashed_password = generate_password_hash(new_password)
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hashed_password, user_id)
        return redirect("/")
    else:
        return render_template("change_password.html")





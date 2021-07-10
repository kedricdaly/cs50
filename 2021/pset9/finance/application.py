import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# -------1---------2---------3---------4---------5---------6---------7---------8


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session['user_id']

    # Get grouped stock information to generate portfolio"
    select_rows = "history.stock_symbol, SUM(quantity) as total_quantity, AVG(purchase_price) as avg_cost, SUM(purchase_price * quantity) as total_cost"
    query = 'SELECT ' + select_rows + ' FROM history '\
            'WHERE fk_user_id = ? '\
            'GROUP BY stock_symbol '\
            'HAVING total_quantity > 0 '\
            'ORDER BY stock_symbol asc'
    rows = db.execute(query, user_id)

    currUser = db.execute('SELECT username, cash FROM users WHERE id = ?', user_id)
    username = currUser[0]['username']
    available_cash = float(currUser[0]['cash'])

    # sum up current value of stocks
    total_stock_value = 0
    for row in rows:
        symbol = row['stock_symbol']
        nShares = row['total_quantity']
        quote = lookup(symbol)
        current_price = quote['price']
        total_stock_value += nShares * current_price
        # add info to rows so it can be passed to html page
        row['current_price'] = current_price
        row['current_value'] = current_price * nShares

    total_value = total_stock_value + available_cash

    return render_template("index.html", rows=rows, username=username, available_cash=available_cash, total_stock_value=total_stock_value, total=total_value)

# -------1---------2---------3---------4---------5---------6---------7---------8


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if symbol == '':
            return apology("Cannot have blank stock")

        quote = lookup(symbol)
        nShares = request.form.get("shares")  # string at this point

        if nShares == '':
            return apology("Must enter a value for shares")
        elif '-' in nShares:
            return apology("Shares should be a positive value")
        elif '.' in nShares:
            return apology("Shares should be a positive integer")
        elif not nShares.isnumeric():
            return apology("Shares must only be numeric")

        # Bad quote
        '''
        if quote == None:
            flash("Invalid stock symbol.")
            return render_template("/buy.html")
        '''
        # per requirements, should show apology instead of flash
        if quote == None:
            return apology("Invalid stock")

        nShares = int(float(nShares))  # converted to int
        price = float(quote['price'])

        total = nShares * price

        if nShares < 1:
            return apology("Shares should be a positive integer")

        rows = db.execute("SELECT cash FROM users WHERE id = ?", session['user_id'])
        if len(rows) != 1:
            return apology("Error during purchase processing")

        available_funds = rows[0]["cash"]

        if available_funds < total:
            return apology("Insufficient funds")

        # Success!
        # Insert into db

        # Update cash amount
        try:
            db.execute("Update users SET cash = ? WHERE id = ?", available_funds - total, session['user_id'])
        except:
            return apology("Error during purchase")

        # Insert into history
        try:
            db.execute("INSERT into HISTORY (fk_user_id, stock_symbol, purchase_price, quantity, timestamp) VALUES (?, ?, ?, ?, datetime('now'))",
                       session['user_id'], quote['symbol'], price, nShares)
        except:
            return apology("Error during purchase")

        flash_msg = 'Purchased ' + str(nShares) + ' share(s) of ' +\
            quote['name'] + ' (' + quote['symbol'] + ') at ' + str(usd(price)) + ' (' + str(usd(total)) + ')'
        #flash_msg = 'Purchased '+ str(nShares) + ' share(s) of ' + quote['name'] + ' (' + quote['symbol'] + ')'
        flash(flash_msg)
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session['user_id'])
        available_funds = rows[0]["cash"]

        if len(rows) != 1:
            return apology("Error during purchase processing")
        return render_template("/buy.html", available_funds=available_funds)

# -------1---------2---------3---------4---------5---------6---------7---------8


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Allow user to deposit funds"""
    user_id = session['user_id']

    if request.method == "POST":
        '''add funds to db'''
        deposit = request.form.get("new_funds")
        if deposit == '':
            return apology("Must enter a value for deposit")
        elif '-' in deposit:
            return apology("Deposit should be a positive value")
        elif '.' in deposit:
            return apology("Deposit should be a positive integer")
        elif not deposit.isnumeric():
            return apology("Deposit must only be numeric")

        deposit = int(deposit)

        rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        if len(rows) != 1:
            apology('Error during funding', "read from db")

        available_funds = rows[0]["cash"]

        try:
            db.execute("Update users SET cash = ? WHERE id = ?", available_funds + deposit, user_id)
        except:
            return apology("Error during funding", "update db")

        flash("Added funds to account")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("/deposit.html")

# -------1---------2---------3---------4---------5---------6---------7---------8


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session['user_id']

    select_rows = "stock_symbol, quantity, purchase_price, quantity * purchase_price as total, timestamp"
    query = 'SELECT ' + select_rows + ' FROM history '\
            'WHERE fk_user_id = ? '\
            'ORDER BY timestamp ASC'
    portfolio = db.execute(query, user_id)

    # Get username for display
    currUser = db.execute('SELECT username FROM users WHERE id = ?', user_id)
    username = currUser[0]['username']

    return render_template("history.html", portfolio=portfolio, username=username)

# -------1---------2---------3---------4---------5---------6---------7---------8


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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

# -------1---------2---------3---------4---------5---------6---------7---------8


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

# -------1---------2---------3---------4---------5---------6---------7---------8


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        quote = lookup(request.form.get("symbol"))

        # Bad quote
        '''
        if quote == None:
            flash("Invalid stock symbol.")
            return render_template("/quote.html")
        '''
        if quote == None:
            return apology("Cannot have empty/invalid stock symbol")

        quote['price'] = usd(quote['price'])

        # Display parameters of stock to user
        return render_template("/quoted.html", stock=quote)

    # reached via Get - display quote submission form
    else:
        return render_template("/quote.html")

# -------1---------2---------3---------4---------5---------6---------7---------8


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Submitted registration attempt
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        elif not request.form.get("confirmation"):
            return apology("password and confirmation not the same", 400)

        # check if username already exists
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        if len(rows) != 0:
            return apology("username already taken", 400)

        # check if password and confirmation are the same
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("password and confirmation not the same", 400)

        # Passed all checks, insert into db.
        pHash = generate_password_hash(request.form.get("password"))
        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?) ", request.form.get("username"), pHash)
            flash("Successfully registered.")
            return render_template('/login.html')
        except:
            flash("Error during registration")
            return render_template('/register.html')

    # reached via GET
    else:
        return render_template("/register.html")

# -------1---------2---------3---------4---------5---------6---------7---------8


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if symbol == '':
            return apology("Cannot have blank stock")

        quote = lookup(symbol)
        nShares = request.form.get("shares")  # string at this point

        if nShares == '':
            return apology("Cannot have blank number of shares")

        # Bad quote
        '''
        if quote == None:
            flash("Invalid stock symbol.")
            return render_template("/buy.html")
        '''
        # per requirements, should show apology instead of flash
        if quote == None:
            return apology("Invalid stock")

        nShares = int(float(nShares))  # converted to int
        price = float(quote['price'])

        total = nShares * price

        if nShares < 1:
            return apology("Shares should be a positive integer")

        # check that they have at least the number of shares they are trying to sell
        # generate portfolio
        select_rows = "history.stock_symbol, SUM(quantity) as total_quantity, AVG(purchase_price) as avg_cost, SUM(purchase_price * quantity) as total_cost"
        query = 'SELECT ' + select_rows + ' FROM history '\
                'WHERE (fk_user_id = ? AND stock_symbol = ?)'\
                'GROUP BY stock_symbol '\
                'HAVING total_quantity > 0 '\
                'ORDER BY stock_symbol asc'
        portfolio = db.execute(query, session['user_id'], quote['symbol'])

        if len(portfolio) < 1:
            return apology("Error during sale processing", "No shares of requested stock")

        available_shares = portfolio[0]['total_quantity']
        if available_shares < nShares:
            return apology("Insufficient shares")

        user_rows = db.execute("SELECT cash FROM users WHERE id = ?", session['user_id'])
        if len(user_rows) > 1:
            return apology("Error during sale processing")

        available_cash = user_rows[0]['cash']
        gained_cash = nShares * price

        # Success!
        # Insert into db

        # Insert into history table
        try:
            db.execute("INSERT into HISTORY (fk_user_id, stock_symbol, purchase_price, quantity, timestamp) VALUES (?, ?, ?, ?, datetime('now'))",
                       session['user_id'], quote['symbol'], price, -nShares)
        except:
            return apology("Error during purchase")

        # Update cash amount
        try:
            db.execute("Update users SET cash = ? WHERE id = ?", available_cash + gained_cash, session['user_id'])
        except:
            return apology("Error during purchase")

        flash_msg = 'Sold ' + str(nShares) + ' share(s) of ' +\
            quote['name'] + ' (' + quote['symbol'] + ') at ' + str(usd(price)) + ' (' + str(usd(total)) + ')'
        flash(flash_msg)
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Get grouped stock information to generate portfolio"
        select_rows = "stock_symbol, SUM(quantity) as total_quantity"
        query = 'SELECT ' + select_rows + ' FROM history '\
                'WHERE fk_user_id = ? '\
                'GROUP BY stock_symbol '\
                'HAVING total_quantity > 0 '\
                'ORDER BY stock_symbol asc'
        portfolio = db.execute(query, session['user_id'])

        return render_template("/sell.html", portfolio=portfolio)


# -------1---------2---------3---------4---------5---------6---------7---------8
def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# -------1---------2---------3---------4---------5---------6---------7---------8
# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

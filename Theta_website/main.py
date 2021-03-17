from flask import *
from flask_mysqldb import MySQL


app = Flask(__name__)

app.config['MYSQL_HOST'] = '127.0.0.1'
app.config['MYSQL_USER'] = 'root'
app.config['MYSQL_PASSWORD'] = ''
app.config['MYSQL_DB'] = 'thetha'

mysql = MySQL(app)


@app.route("/")
def home():
    cur = mysql.connection.cursor()
    cur.execute("""SELECT direction FROM dir WHERE id = 1""", )
    user = cur.fetchone()
    return render_template("home.html", x=user[0], y="5")


@app.route("/about")
def direction():
    cur = mysql.connection.cursor()
    cur.execute("""SELECT direction FROM dir WHERE id = 1""", )
    user = cur.fetchone()
    return render_template('about.html', test=user)
    mysql.close()



@app.route('/stream')
def stream():

    return render_template('video.html')








if __name__ == "__main__":
    app.run(host='192.168.1.13', debug=True)




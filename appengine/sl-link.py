import os
from google.appengine.ext.webapp import template

from google.appengine.api import users
from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app
from google.appengine.ext import db

main_key = "agdzbC1saW5rchMLEglEaXJlY3Rpb24iBG1haW4M"

class Direction(db.Model):
  label = db.StringProperty()
  player1_dir = db.StringProperty()
  player2_dir = db.StringProperty()
  player1_hr = db.StringProperty()
  player2_hr = db.StringProperty()
  player1_pos = db.StringProperty()
  player2_pos = db.StringProperty()

class MainPage(webapp.RequestHandler):
  def get(self):
    path = os.path.join(os.path.dirname(__file__), 'index.html')
    self.response.out.write('zoom')

class SetData(webapp.RequestHandler):
  def get(self):
    obj = db.get(db.Key(main_key))
    args = self.request.arguments()
    for a in args:
      if a == "p1d":
        obj.player1_dir = self.request.get("p1d")
      if a == "p2d":
        obj.player2_dir = self.request.get("p2d")
      if a == "p1hr":
        obj.player1_hr = self.request.get("p1hr")
      if a == "p2hr":
        obj.player2_hr = self.request.get("p2hr")
      if a == "p1p":
        obj.player1_pos = self.request.get("p1p")
      if a == "p2p":
        obj.player2_pos = self.request.get("p2p")
    obj.put()

class GetData(webapp.RequestHandler):
  def get(self):
    obj = db.get(db.Key(main_key))
    args = self.request.arguments()
    self.response.headers['Content-Type'] = 'text/plain'
    for a in args:
      if a == "dir":
        self.response.out.write(obj.direction)
      if a == "hr":
        self.response.out.write(obj.heartrate)
      if a == "p1p":
        self.response.out.write(obj.player1_pos)
      if a == "p2p":
        self.response.out.write(obj.player2_pos)

class GetAll(webapp.RequestHandler):
  def get(self):
    obj = db.get(db.Key(main_key))
    out = obj.player1_pos + "," + obj.player1_hr + "," + obj.player2_pos + "," + obj.player2_hr
    self.response.out.write(out)

class Init(webapp.RequestHandler):
  def get(self):
    d = Direction(key_name = "main")
    d.put()
    d_key = d.key() 
    self.response.headers['Content-Type'] = 'text/plain'
    self.response.out.write(d_key)

application = webapp.WSGIApplication(
                                     [('/', MainPage),
                                     ('/set_data',SetData),
                                      ('/get_data',GetData),
                                      ('/init', Init),
                                      ('/get_all',GetAll)],
                                      debug=True)

def main():
  run_wsgi_app(application)

if __name__ == "__main__":
  main()

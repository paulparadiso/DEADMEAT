import os
from google.appengine.ext.webapp import template

from google.appengine.api import users
from google.appengine.ext import webapp
from google.appengine.ext.webapp.util import run_wsgi_app
from google.appengine.ext import db

main_key = "agdzbC1saW5rchMLEglEaXJlY3Rpb24iBG1haW4M"

class Direction(db.Model):
  label = db.StringProperty()
  direction = db.StringProperty()
  heartrate = db.StringProperty()

class MainPage(webapp.RequestHandler):
  def get(self):
    path = os.path.join(os.path.dirname(__file__), 'index.html')
    self.response.out.write('zoom')

class SetData(webapp.RequestHandler):
  def get(self):
    obj = db.get(db.Key(main_key))
    args = self.request.arguments()
    for a in args:
      if a == "dir":
        obj.direction = self.request.get("dir")
      if a == "hr":
        obj.heartrate = self.request.get("hr")
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
                                      ('/init', Init)],
                                     debug=True)

def main():
  run_wsgi_app(application)

if __name__ == "__main__":
  main()

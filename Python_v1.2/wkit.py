import gi
gi.require_version('Gtk', '3.0')
gi.require_version('WebKit2', '4.0')

import os

from gi.repository import Gtk, WebKit2

# file_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "teste.html"))
# local_url = QUrl.fromLocalFile(file_path)
# browser.load(local_url)

# get parent folder with `os.path`
# import os.path
# BASE_DIR = os.path.dirname(os.path.abspath(__file__))
# file = "teste.html"
# final_path = os.path.join(BASE_DIR, file)

# get parent folder with `pathlib`'s Path
from pathlib import Path
BASE_DIR = Path(__file__).absolute().parent
file = "teste.html"
final_path = BASE_DIR / file
final_path = "file://" + str(final_path) + "?dados=saoos"

print(final_path)


window = Gtk.Window()
window.set_default_size(800, 600)
window.connect("destroy", Gtk.main_quit)

scrolled_window = Gtk.ScrolledWindow()
webview = WebKit2.WebView()
webview.load_uri(final_path)
scrolled_window.add(webview)

window.add(scrolled_window)
window.show_all()
Gtk.main()
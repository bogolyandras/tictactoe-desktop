
//TicTacToe core library
#include <layout.h>
#include <table.h>
#include <ai/heuristicai.h>

#include <gtkmm.h>

class MyWindow : public Gtk::Window
{
public:
  MyWindow();
};

MyWindow::MyWindow()
{
  set_title("TicTacToe");
  set_default_size(200, 200);
}

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("com.bogolyandras.tictactoe");

  return app->make_window_and_run<MyWindow>(argc, argv);
}

#import_function :window

def hello
  p "hello ruby on browzer"
  p Time.now
  #  window.alert "hello prompt"
end

class Hoge
  def method_missing(method, *params)
    p method
    p params
  end
end

#$j.window.alert("a")

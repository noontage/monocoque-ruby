#import_function :window

def hello
  p "hello ruby on browzer"
  p Time.now
  #  window.alert "hello prompt"
end

def j
  JavaScriptObject.new
end

puts j.Math.pow(2, 3).var
p j.document.getElementById("test123").textContent = "OK!!!"

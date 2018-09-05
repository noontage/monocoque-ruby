# encoding: utf-8

def hello
  p "hello ruby on browzer"
  p "1234"
  p "日本語"
  p Time.now
end

def j
  JavaScriptObject.new
end

j.setTimeout(-> {
  a = 1
  b = 9
  c = a + b
  j.alert "Welcome"
}, 1000)

hello

puts j.Math.pow(2, 3).get_value
j.document.getElementById("loading").textContent = "OK"

link1 = j.document.getElementById("link1")

link1.addEventListener("click", -> event {
  j.alert "onclick"
}, false)

j.window.addEventListener("keypress", -> event {
  a = 1
  b = 9
  c = a + b
  if (c == 10)
    j.console.log(event.keyCode._val)
  end
}, false)

def hello
  p "hello ruby on browzer"
  p Time.now
end

def j
  JavaScriptObject.new
end

puts j.Math.pow(2, 3).get_value
j.document.getElementById("loading").textContent = "OK"

link1 = j.document.getElementById("link1")
link1.addEventListener("click", -> { j.alert "CLICKED" }, false)
link1.addEventListener("mouseover", -> { j.alert "ONMOUSE" }, false)

# j.setTimeout(-> { puts "OK" }, 1500)

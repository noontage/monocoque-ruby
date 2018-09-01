def hello
  p "hello ruby on browzer"
  p Time.now
end

def j
  JavaScriptObject.new
end

#puts j.Math.pow(2, 3).var
j.document.getElementById("loading").textContent = "OK"

#link = j.document.getElementById("link1")
#link.addEventListener?("click", -> { j.alert "CLICKED" }, false)
#link.addEventListener("mouseover", -> { j.alert "ONMOUSE" }, false)
# j.setTimeout(-> { puts "OK" }, 1500)

def hello
  p "hello ruby on browzer"
  p Time.now
end

def j
  JavaScriptObject.new
end

#puts j.Math.pow(2, 3).var
#j.alert
#p j.document.getElementById("test123").textContent = "OK!!!"
#j.test
#j.document.getElementById("test123")

j.setTimeout(-> { j.alert "OK" }, 1500)

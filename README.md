# Def
def

### DEFine a variable
```
def name = value
```

### DEFine a function
```
def main(def param1 = 0 def param2 = 3 def param3 = def type) {
    print((param1 + param2) / param3)
}
```

### DEFine a type
```
def MyType {
    def field1 = 0
    def field2 = 100
}
```

### DEFine a return value
```
def addAB(def a = 0, def b = 0) {
  def a + b
}
```

### DEFine a new instance of a type
```
def variable = def MyType
print(variable.field1)
print(variable.field1 + variable.field2)
variable.method()
print(variable.field1)
```

### DEFine access to this
```
def MyType {
    def field1 = 0
    def field2 = 100

    def method() {
        def.field1 = def.field1 + def.field2
        def def.field1 * def.field2
    }
}
```

### DEFine a stub function
```
def print(var value = 0)
```

### Running DEF
```
def /path/to/source.def
```

### Example DEF file
```
def print(def var = 0)

def type {
	def z = 0
	def k = 0

	def hello(def z = 0) {
		def.z = 1
		def.k = def.z + def.k

		def def.k
	}
}

def main() {
	def y = 6

	def k = def type
	k.hello()
	k.hello()

	print(y + k.k  + k.hello())
}
```

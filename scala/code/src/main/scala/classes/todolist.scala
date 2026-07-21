package todolist
import scala.collection.mutable.Map

class ToDoList(
  var items: Map[Int, ToDoItem]
) {
  var id = 0
  def newID = () =>
    id += 1
    id

  def addItem = (item: ToDoItem) =>
    items(newID()) = item
  
  def removeItemByID = (id: Int) =>
    items -= id
  
  def removeAllItems = () =>
    items.clear()
  
  def getItems = () =>
    items.values.toList
  
  def copy = () =>
    ToDoList(items.clone())
  
  override def equals(obj: Any): Boolean = 
    obj match {
      case other: ToDoList =>
        this.items == other.items
      case _ => false
    }
}

object ToDoList {
  def empty() = 
    ToDoList(Map.empty[Int, ToDoItem])
}

case class ToDoItem (
  content: String
)

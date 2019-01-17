package Assignment

import java.io.BufferedWriter
import java.io.FileWriter

import scala.collection.JavaConversions._
import scala.collection.mutable.ListBuffer

import au.com.bytecode.opencsv.CSVWriter

object CSV_writer extends App {
  
  def writeCsv(fileName: String, headers: Array[String], values: Array[Array[(Double)]]) = {
    // Create file, where the data will be written
    val outputFile = new BufferedWriter(new FileWriter("results/" + fileName + ".csv"))
    val separator  = CSVWriter.DEFAULT_SEPARATOR
    val quotechar = CSVWriter.NO_QUOTE_CHARACTER
    val escapechar = CSVWriter.DEFAULT_ESCAPE_CHARACTER
    val lineEnd = CSVWriter.DEFAULT_LINE_END
    val csvWriter = new CSVWriter(outputFile, separator, quotechar, escapechar, lineEnd)
    
    val linesToWrite = new ListBuffer[Array[String]]()
    // Add headers
    linesToWrite += headers
    
    // Convert row Array[Double] to Array[String] 
    for (row <- values) {
      val stringRow = new Array[String](row.length)
      for (i <- 0 until row.length) {
        stringRow(i) = row(i).toString()
      }
      linesToWrite += stringRow
    }
    
    // Write the headers and data to the file
    csvWriter.writeAll(linesToWrite.toList)
    outputFile.close()    
  }
}
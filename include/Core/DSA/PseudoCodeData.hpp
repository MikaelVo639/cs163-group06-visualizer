#pragma once
#include <vector>
#include <string>

namespace Core::DSA::PseudoCode {

    // ===== LINKED LIST =====

    inline std::vector<std::string> insertHead() {
        return {
            "newNode = new Node(val)",
            "newNode.next = head",
            "head = newNode"
        };
    }

    inline std::vector<std::string> insertTail() {
        return {
            "if head == null:",
            "  head = new Node(val)",
            "else:",
            "  curr = head",
            "  while curr.next != null:",
            "    curr = curr.next",
            "  curr.next = new Node(val)"
        };
    }

    inline std::vector<std::string> insertAt() {
        return {
            "if pos < 0 or pos > size: return",
            "if pos == 0: insertHead(val)",
            "else:",
            "  pre = head",
            "  for i = 0 to pos - 2:",
            "    pre = pre.next",
            "  newNode = new Node(val)",
            "  newNode.next = pre.next",
            "  pre.next = newNode"
        };
    }

    inline std::vector<std::string> deleteHead() {
        return {
            "if head == null: return",
            "temp = head",
            "head = head.next",
            "delete temp"
        };
    }

    inline std::vector<std::string> deleteTail() {
        return {
            "if head == null: return",
            "if head.next == null: head = null",
            "else:",
            "  pre = head",
            "  while pre.next.next != null:",
            "    pre = pre.next",
            "  del = pre.next",     // Line 6
            "  pre.next = null",    // Line 7
            "  delete del"          // Line 8
        };
    }

    inline std::vector<std::string> deleteAt() {
        return {
            "if pos < 0 or pos >= size: return",
            "if pos == 0: deleteHead()",
            "else:",
            "  pre = head",
            "  for i = 0 to pos - 2:",
            "    pre = pre.next",
            "  del = pre.next",
            "  pre.next = del.next",
            "  delete del"
        };
    }

    inline std::vector<std::string> search() {
        return {
            "curr = head",
            "while curr != null:",
            "  if curr.val == target:",
            "    return FOUND",
            "  curr = curr.next",
            "return NOT FOUND"
        };
    }

    inline std::vector<std::string> updateAt() {
        return {
            "if pos < 0 or pos >= size: return",
            "curr = head",
            "for i = 0 to pos - 1:",
            "  curr = curr.next",
            "curr.val = newVal"
        };
    }

    inline std::vector<std::string> updateByValue() {
        return {
            "curr = head",
            "while curr != null:",
            "  if curr.val == oldVal:",
            "    curr.val = newVal",
            "    return FOUND",
            "  curr = curr.next",
            "return NOT FOUND"
        };
    }

} // namespace Core::DSA::PseudoCode

# custom message : Add LongLP to detect message from me
function (longlp_message)
  set (message_type_ STATUS WARNING AUTHOR_WARNING SEND_ERROR FATAL_ERROR DEPRECATION)
  if (NOT ${ARGV0} IN_LIST message_type_)
    # default message is STATUS type
    list (INSERT ARGV 0 STATUS)
  endif ()
  list (INSERT ARGV 1 "LongLP: ")
  message (${ARGV})
endfunction ()

function composing():
    job.bash("echo composing")
    job.bash("ls -l")
end

function action()
    job.bash("echo hello")
    job.bash("ls -l")
end

function result()
    job.bash("echo result")
    job.bash("ls -l")
end

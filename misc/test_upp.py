import pytest
import os
import subprocess
from pathlib import Path

# Constants
TEST_REPO = "test_repo"
PROJECT_ROOT = Path(__file__).parent.parent
BUILD_DIR = PROJECT_ROOT / "build"
UPP_BINARY = BUILD_DIR / "upp"

@pytest.fixture(scope="module")
def build_upp():
    """Build the C++ executable using CMake."""
    BUILD_DIR.mkdir(exist_ok=True)
    subprocess.run(["cmake", ".."], cwd=BUILD_DIR, check=True)
    subprocess.run(["make"], cwd=BUILD_DIR, check=True)
    yield
    # Optional cleanup
    # subprocess.run(["make", "clean"], cwd=BUILD_DIR)

@pytest.fixture
def test_repo(tmp_path, build_upp):
    """Create a fresh test repository."""
    repo = tmp_path / TEST_REPO
    repo.mkdir()
    
    # Initialize repo
    subprocess.run([str(UPP_BINARY), "init"], cwd=repo, check=True)
    return repo

# ---- Core Functionality Tests ----
def test_add_single_file(test_repo):
    """Test adding a single file."""
    test_file = test_repo / "hello.txt"
    test_file.write_text("Test content")
    
    result = subprocess.run(
        [str(UPP_BINARY), "add", "hello.txt"],
        cwd=test_repo,
        capture_output=True,
        text=True
    )
    
    assert result.returncode == 0
    index = (test_repo / ".upl/index").read_text()
    assert "hello.txt" in index
    assert "blob" in index  # Verify proper index entry

def test_add_multiple_files(test_repo):
    """Test adding multiple files at once."""
    files = ["file1.txt", "file2.txt"]
    for f in files:
        (test_repo / f).write_text(f"Content for {f}")
    
    result = subprocess.run(
        [str(UPP_BINARY), "add", *files],
        cwd=test_repo,
        capture_output=True,
        text=True
    )
    
    assert result.returncode == 0
    index = (test_repo / ".upl/index").read_text()
    for f in files:
        assert f in index

def test_add_all_files(test_repo):
    """Test adding all files with '.' wildcard."""
    files = ["a.txt", "b.txt"]
    for f in files:
        (test_repo / f).write_text("content")
    
    result = subprocess.run(
        [str(UPP_BINARY), "add", "."],
        cwd=test_repo,
        capture_output=True,
        text=True
    )
    
    assert result.returncode == 0
    index = (test_repo / ".upl/index").read_text()
    for f in files:
        assert f in index

def test_commit_basic(test_repo):
    """Test basic commit functionality."""
    # Setup: Add a file
    test_file = test_repo / "commit_test.txt"
    test_file.write_text("Commit this")
    subprocess.run([str(UPP_BINARY), "add", "commit_test.txt"], cwd=test_repo, check=True)
    
    # Commit
    commit_msg = "Test commit"
    result = subprocess.run(
        [str(UPP_BINARY), "commit", "-m", commit_msg],
        cwd=test_repo,
        capture_output=True,
        text=True
    )
    
    assert result.returncode == 0
    assert "commit" in result.stdout.lower()
    
    # Verify objects were created
    objects_dir = test_repo / ".upl/objects"
    assert any(objects_dir.glob("*/*"))  # Check for object files

def test_commit_without_add(test_repo):
    """Test commit fails with no staged files."""
    result = subprocess.run(
        [str(UPP_BINARY), "commit", "-m", "Should fail"],
        cwd=test_repo,
        capture_output=True,
        text=True
    )
    
    assert result.returncode != 0
    assert "error" in result.stderr.lower()

# ---- Edge Cases ----
def test_add_nonexistent_file(test_repo):
    """Test error handling for non-existent files."""
    result = subprocess.run(
        [str(UPP_BINARY), "add", "ghost_file.txt"],
        cwd=test_repo,
        capture_output=True,
        text=True
    )
    
    assert result.returncode != 0
    assert "error" in result.stderr.lower()

def test_empty_commit_message(test_repo):
    """Test commit with empty message."""
    test_file = test_repo / "empty.txt"
    test_file.write_text("content")
    subprocess.run([str(UPP_BINARY), "add", "empty.txt"], cwd=test_repo, check=True)
    
    result = subprocess.run(
        [str(UPP_BINARY), "commit", "-m", ""],
        cwd=test_repo,
        capture_output=True,
        text=True
    )
    
    assert result.returncode != 0
    assert "empty" in result.stderr.lower()

# ---- Verification Tests ----
def test_index_after_add(test_repo):
    """Verify index format after add."""
    test_file = test_repo / "index_test.txt"
    test_file.write_text("index test")
    subprocess.run([str(UPP_BINARY), "add", "index_test.txt"], cwd=test_repo, check=True)
    
    index = (test_repo / ".upl/index").read_text()
    parts = index.split()
    assert len(parts) >= 4  # Format: mode type hash path
    assert parts[0] == "100644"
    assert parts[1] == "blob"

def test_object_creation(test_repo):
    """Verify objects are created with correct hashes."""
    test_file = test_repo / "obj_test.txt"
    test_file.write_text("object test")
    subprocess.run([str(UPP_BINARY), "add", "obj_test.txt"], cwd=test_repo, check=True)
    
    index = (test_repo / ".upl/index").read_text()
    hash_value = index.split()[2]  # Get hash from index
    obj_path = test_repo / f".upl/objects/{hash_value[:2]}/{hash_value[2:]}"
    
    assert obj_path.exists()
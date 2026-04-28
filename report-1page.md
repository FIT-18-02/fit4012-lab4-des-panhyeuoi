# Report 1 page - Lab 4 DES / TripleDES

## Mục tiêu

Mục tiêu của bài lab này là tìm hiểu và trực tiếp thực thi thuật toán mã hóa khối chuẩn DES (Data Encryption Standard) và biến thể an toàn hơn là TripleDES. Qua đó, sinh viên nắm vững cấu trúc mạng Feistel, các bước hoán vị, thay thế (S-box), cơ chế quản lý khóa vòng, cũng như cách xử lý dữ liệu thực tế thông qua kỹ thuật Padding và mã hóa đa khối (Multi-block).

## Cách làm / Method

Em đã thực hiện các thay đổi và bổ sung quan trọng sau trên nền tảng code gốc:

- Cấu trúc lại mã nguồn: Chia chương trình thành các Class chuyên biệt như KeyGenerator (quản lý tạo 16 khóa vòng) và DES (xử lý cốt lõi thuật toán).

- Bổ sung chức năng Padding: Triển khai cơ chế Zero Padding để đảm bảo mọi đầu vào đều được đưa về bội số của 64 bit trước khi mã hóa.

- Xử lý Multi-block: Sử dụng vòng lặp để cắt dữ liệu dài thành từng khối 64 bit và xử lý tuần tự, cho phép mã hóa các chuỗi văn bản dài.

- Triển khai TripleDES: Thiết lập quy trình mã hóa theo mô hình chuẩn EDE (Encrypt K1 -> Decrypt K2 -> Encrypt K3) và quy trình giải mã ngược lại.

- Giao diện dòng lệnh: Hoàn thiện hàm main tuân thủ đúng "Submission Contract" để nhận Mode và dữ liệu từ stdin.

## Kết quả / Result

Chương trình đã vượt qua các bài kiểm tra quan trọng:

- Mã hóa DES: Chạy đúng với các Test Vector chuẩn, kết quả đầu ra là chuỗi nhị phân khớp với tài liệu kỹ thuật của NIST.

- Giải mã (Round-trip): Thực hiện mã hóa một chuỗi bit bất kỳ, sau đó dùng kết quả đó để giải mã với cùng khóa, kết quả thu được khớp hoàn toàn với bản rõ ban đầu.

- TripleDES: Kiểm tra thành công với 3 khóa khác nhau, đảm bảo tính đúng đắn của quy trình EDE.

- Negative Tests: Khi cố tình nhập sai khóa hoặc thay đổi dù chỉ 1 bit trong ciphertext (tamper), chương trình giải mã ra kết quả hoàn toàn khác biệt, chứng minh tính toàn vẹn của thuật toán.

## Kết luận / Conclusion

Thông qua bài lab, em đã hiểu rõ cơ chế hoạt động chi tiết bên trong của DES và cách mà các phép toán đơn giản (XOR, hoán vị) khi kết hợp lại có thể tạo ra một hệ thống bảo mật phức tạp.

- Hạn chế: Cơ chế Zero Padding hiện tại có thể gây nhầm lẫn nếu dữ liệu gốc kết thúc bằng bit 0.

- Hướng mở rộng: Trong tương lai có thể nâng cấp lên cơ chế đệm PKCS#7 để an toàn hơn và triển khai các chế độ mã hóa như CBC (Cipher Block Chaining) để tránh việc các khối bản rõ giống nhau cho ra bản mã giống nhau.

#pragma once

namespace InventoryManagement {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ReportForm
	/// </summary>
	public ref class ReportForm : public System::Windows::Forms::Form
	{
	public:
		ReportForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ReportForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::TextBox^  txtReport;
	private: System::Windows::Forms::Button^  btnReportDone;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->txtReport = (gcnew System::Windows::Forms::TextBox());
			this->btnReportDone = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// txtReport
			// 
			this->txtReport->Font = (gcnew System::Drawing::Font(L"Courier New", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->txtReport->Location = System::Drawing::Point(13, 13);
			this->txtReport->Multiline = true;
			this->txtReport->Name = L"txtReport";
			this->txtReport->ReadOnly = true;
			this->txtReport->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtReport->Size = System::Drawing::Size(709, 397);
			this->txtReport->TabIndex = 0;
			this->txtReport->TabStop = false;
			// 
			// btnReportDone
			// 
			this->btnReportDone->Location = System::Drawing::Point(647, 427);
			this->btnReportDone->Name = L"btnReportDone";
			this->btnReportDone->Size = System::Drawing::Size(75, 23);
			this->btnReportDone->TabIndex = 1;
			this->btnReportDone->Text = L"Done";
			this->btnReportDone->UseVisualStyleBackColor = true;
			this->btnReportDone->Click += gcnew System::EventHandler(this, &ReportForm::btnReportDone_Click);
			// 
			// ReportForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(734, 462);
			this->Controls->Add(this->btnReportDone);
			this->Controls->Add(this->txtReport);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"ReportForm";
			this->Text = L"Report";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btnReportDone_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }
	};
}
